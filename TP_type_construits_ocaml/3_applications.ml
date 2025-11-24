type int_list = Head of int*int_list | End;;

let rec convert_to_int_list ls =
  let tail = List.tl ls in
  let hd = List.hd ls in
  let len = List.length ls in
  if len = 1 then
    Head(hd, End)
  else
    Head(hd, convert_to_int_list tail);;

let rec convert_to_ocaml_list ls =
  match ls with
  |Head(e, tail) -> e::(convert_to_ocaml_list tail)
  |End -> [];;

let get_head ls =
  match ls with
  |Head(e, tail) -> Some(e)
  |End -> None

let rec is_in_list ls e =
  match ls with
  |Head(v, tail) when v = e -> true
  |Head(v, tail) -> is_in_list tail e
  |End -> false;;

let rec is_a_nb_after_zero ls =
  match ls with
  |Head(e, tail) when e = 0 -> get_head tail
  |Head(e, tail) -> is_a_nb_after_zero tail
  |End -> None;;

let ocaml_ls = [1; 2; 3; 0; 4; 5];;
let my_int_ls = convert_to_int_list ocaml_ls;;
let back_to_ocaml_ls = convert_to_ocaml_list my_int_ls;;
let hd = get_head my_int_ls;;
let is_2 = is_in_list my_int_ls 2;;
let is_9 = is_in_list my_int_ls 9;;
let nb_after_zero = is_a_nb_after_zero my_int_ls;;

type expression =
  Value of float
 |Sum of expression * expression
 |Prod of expression * expression
 |Sub of expression * expression
 |Div of expression * expression
 |NaN;;

let e = Sum(Value(1.), Prod(Value(5.5), Sum(Value(2.), Value(3.))));;

let rec evaluate e =
  match e with
  |Sum(e1, e2) -> evaluate e1 +. evaluate e2
  |Prod(e1, e2) -> evaluate e1 *. evaluate e2
  |Sub(e1, e2) -> evaluate e1 -. evaluate e2
  |Value(v) -> v;;

let e_value = evaluate e;;

let rec sum_operator k n a =
  let e = a k in
  match k with
  |k when k >= n -> e
  |_ -> Sum(e, sum_operator (k +. 1.) n a);;

let sum_0_100 = sum_operator 0. 100. (fun x -> Value(x *. x));;
let value_of_sum = evaluate sum_0_100;;

let rec simplify e =
  let simplify_with_NaN e1 e2 op =
    let s1 = simplify e1 in
    let s2 = simplify e2 in
    match s1, s2 with
    |Value(v1), Value(v2) -> Value(op v1 v2)
    |NaN, _ -> NaN
    |_, NaN -> NaN
    |_, _ -> NaN
  in
  match e with
  |NaN -> NaN
  |Sum(e1, e2) -> simplify_with_NaN e1 e2 (fun v1 -> fun v2 -> v1 +. v2)
  |Prod(e1, e2) -> simplify_with_NaN e1 e2 (fun v1 -> fun v2 -> v1 *. v2)
  |Sub(e1, e2) -> simplify_with_NaN e1 e2 (fun v1 -> fun v2 -> v1 -. v2)
  |Div(e1, e2) when simplify e2 = Value(0.) -> NaN
  |Div(e1, e2) -> simplify_with_NaN e1 e2 (fun v1 -> fun v2 -> v1 /. v2)
  |Value(v) -> Value(v);;

let e2 = Prod(Value(5.), Div(Sum(Value(2.), Value(12.)), Value(8.)));;
let e3 = Prod(Value(5.), Div(Sum(Value(2.), Value(12.)), Value(0.)));;
let e4 = Prod(Value(5.), Div(Sum(Value(2.), Value(12.)), Sub(Value(1.), Value(1.))));;

let e2_simplified = simplify e2;;
let e3_simplified = simplify e3;;
let e4_simplified = simplify e4;;
