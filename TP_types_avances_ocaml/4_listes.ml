let rec range n =
  if n = 0 then
    []
  else
    (n-1)::(range (n-1));;

let range_0_4 = range 5;;

let rec is_in_list e ls =
  if List.length ls = 0 then
    false
  else if e = (List.hd ls) then
    true
  else
    is_in_list e (List.tl ls);;

let range_4_in_4 = is_in_list 5 range_0_4;;

let rec list_couples ls =
  let len = List.length ls in
  let couple = (List.hd ls, List.hd ls) in
  if len = 1 then
    [couple]
  else
    couple::list_couples (List.tl ls);;

let range_couples = list_couples range_0_4;;
