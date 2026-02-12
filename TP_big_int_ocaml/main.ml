type big_int = {base: int; digits: int list};;

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

let rec big_int_of_int n b =
  if n = 0 then
    {base=b; digits=[]}
  else
    let q, r = euc_div n b in
    let result = big_int_of_int q b in
    {base=b; digits=r::(result.digits)};;

let int_of_big_int bint =
  let rec f bint i =
    match bint.digits with
    | [] -> 0
    | c::tl ->
       c * (pow bint.base i)
       + f {base=bint.base; digits=tl} (i+1)
  in
  f bint 0;;

let a = 37;;
let ba = big_int_of_int a 2;;
let re_a = int_of_big_int ba;;

exception NotInTheSameBase;;

let rec big_int_sum ba bb carry =
  match ba.digits, bb.digits, carry with
  | [], left, 0 | left, [], 0 -> left
  | [], [], c ->
     let new_c, d = euc_div (a + c) ba.base in
     {base=ba.base; digits=d::(
       big_int_sum
         ba
         bb
         new_c
     )}
  | a::ad, [], c ->
     let new_c, d = euc_div (a + c) ba.base in
     {base=ba.base; digits=d::(
       big_int_sum
         {base=ba.base; digits=ad}
         bb
         new_c
     )}
  | [], b::bd, c ->
     let new_c, d = euc_div (a + c) ba.base in
     {base=ba.base; digits=d::(
       big_int_sum
         ba
         {base=bb.base; digits=bd}
         new_c
     )}
  |a::ad, b::bd, c ->
    let new_c, d = euc_div (a + c) ba.base in
     {base=ba.base; digits=d::(
       big_int_sum
         {base=ba.base; digits=ad}
         {base=bb.base; digits=bd}
         new_c
     )}
          
