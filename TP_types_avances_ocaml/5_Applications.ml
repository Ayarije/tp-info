type complex = {
    x: float;
    y: float;
  };;

let make_complex x y = {
    x = x;
    y = y;
  };;

let sq_module_cplx z = z.x**2. +. z.y**2.;;

let get_conjugue z = {
    x = z.x;
    y = -1. *. z.y;
  };;

let mul_cplx z1 z2 = {
    x = z1.x *. z2.x -. z1.y *. z2.y;
    y = z1.y *. z2.x +. z1.x *. z2.y;
  };;

let div_cplx z1 z2 = {
    x = (mul_cplx z1 (get_conjugue z2)).x /. sq_module_cplx z2;
    y = (mul_cplx z1 (get_conjugue z2)).y /. sq_module_cplx z2;
  };;

let z1 = {
    x = 10.;
    y = 15.;
  };;

let z2 = {
    x = 10.;
    y = 8.;
  };;

let z_mul = mul_cplx z1 z2;;
let z_div = div_cplx z1 z2;;

let sum_couples a b =
  let (a1, a2) = a in
  let (b1, b2) = b in
  (a1 + b1, a2 +. b2);;

let c1 = (5, 2.);;
let c2 = (1, 6.);;

let c_sum = sum_couples c1 c2;;

let rec sum_list ls =
  let len = List.length ls in
  let e = List.hd ls in
  if len = 1 then
    e
  else
    e + sum_list (List.tl ls);;

let ls = [2; 5; 8; 3; 0];;
let ls_sum = sum_list ls;;

let rec even_rank_lst ls =
  let len = List.length ls in
  let e = List.hd ls in
  if len = 1 then
    if e mod 2 = 0 then
      [e]
    else
      []
  else
    if e mod 2 = 0 then
      e::(even_rank_lst (List.tl ls))
    else
      even_rank_lst (List.tl ls);;


let even_ls = even_rank_lst ls;;

let rec zip ls1 ls2 =
  let len1 = List.length ls1 in
  let len2 = List.length ls2 in
  let couple = (List.hd ls1, List.hd ls2) in
  let tl1 = List.tl ls1 in
  let tl2 = List.tl ls2 in
  if len1 <> len2 then
    []
  else if len1 = 1 then
    [couple]
  else
    couple::(zip tl1 tl2);;

let l1 = [0; 1; 2; 3; 4; 5];;
let l2 = [5; 4; 3; 2; 1; 0];;

let zipped_ls = zip l1 l2;;

let rec list_map func ls =
  let len = List.length ls in
  let e = List.hd ls in
  let tl = List.tl ls in
  if len = 1 then
    [func e]
  else
    (func e)::(list_map func tl);;

let f = fun x -> x*2;;

let mapped_ls = list_map f ls;;
