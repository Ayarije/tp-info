type big_int = {base: int; digits: int list; positive: bool};;

let rec pow a n =
  if n = 0 then
    1
  else if n mod 2 = 0 then
    pow (a*a) (n/2)
  else
    a * pow a (n-1);;

let rec euc_div a b =
  if b > a then
    (0, a)
  else
    let q, r = euc_div (a-b) b in
    (q+1, r);;

let _ = euc_div 13 5;;

let rec positive_big_int_of_int n b =
  if n = 0 then
    {base=b; digits=[]; positive=true}
  else
    let q, r = euc_div n b in
    let result = positive_big_int_of_int q b in
    {base=b; digits=r::(result.digits); positive=true};;
let big_int_of_int n b =
  if  n < 0 then
    let bint = positive_big_int_of_int (-n) b in
    {base=b; digits=bint.digits; positive=false}
  else
    positive_big_int_of_int n b;;

let int_of_big_int bint =
  let rec f bint i =
    match bint.digits with
    | [] -> 0
    | c::tl ->
       c * (pow bint.base i)
       + f {base=bint.base; digits=tl; positive=bint.positive} (i+1)
  in
  let sign = if bint.positive then 1 else -1 in
  sign * (f bint 0);;

let a = -37;;
let ba = big_int_of_int a 2;;
let re_a = int_of_big_int ba;;

exception NotInTheSameBase;;

let big_int_get_tl bint =
  if bint.digits = [] then
    (0, bint)
  else    
    (List.hd bint.digits, {base=bint.base; digits=(List.tl bint.digits); positive=bint.positive});;


let rec _big_int_sum ba bb carry =
  match ba.digits, bb.digits, carry with
  | [], left, 0 | left, [], 0 -> {base=ba.base; digits=left; positive=true}
  | _, _, c ->
     let a, a_tl = big_int_get_tl ba in
     let b, b_tl = big_int_get_tl bb in
     let new_carry, d = euc_div (a + b + c) ba.base in
     {
       base=ba.base;
       digits=d::(_big_int_sum a_tl b_tl new_carry).digits;
       positive=true
     };;  
let big_int_sum ba bb = _big_int_sum ba bb 0;;
          
let b = 20;;
let c = 13;;
let bb = big_int_of_int b 2;;
let bc = big_int_of_int c 2;;
let bsum = big_int_sum bb bc;;
let sum = int_of_big_int bsum;;

let rec _big_int_compare_positive a_dig b_dig = (* On considère le plus gros coeff en tête et de même taille *)
  match a_dig, b_dig with
  | [], [] | [], _ | _, [] -> 0
  | a::a_tl, b::b_tl ->
     if a > b then 1
     else if a < b then -1
     else
       _big_int_compare_positive a_tl b_tl;;

let big_int_compare_positive ba bb =
  let a_sz = List.length ba.digits in
  let b_sz = List.length bb.digits in
  if a_sz <> b_sz then
    a_sz - b_sz
  else
    _big_int_compare_positive (List.rev ba.digits) (List.rev bb.digits);;
    
let _ = big_int_compare_positive bb bc;;

let compare_big_int ba bb =
  if ba.positive && not bb.positive then (1)
  else if not ba.positive && bb.positive then (-1)
  else if ba.positive && bb.positive then (big_int_compare_positive ba bb)
  else -(big_int_compare_positive ba bb);;

let _ = compare_big_int ba bb;;
