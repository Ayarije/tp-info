type float_option = None | Some of int;;

let one_divided_by_sqrt = fun x ->
  if x <= 0.0 then
    None
  else
    Some(1.0 /. sqrt x);;

let x5 = one_divided_by_sqrt 5.;;
let x_minus_5 = one_divided_by_sqrt (-5.);;
