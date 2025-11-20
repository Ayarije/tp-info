type int_list = Head of int*list | End;;

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

type expression =
  Value of float
 |Sum of expression * expression
 |Prod of expression * expression
 |Sub of expression * expression;;

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
  |k when k = n -> e
  |_ -> Sum(e, sum_operator (k+1) n a);;

