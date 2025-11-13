let is_bisextil year =
  match year with
  |y when (y mod 4 = 0) && (y mod 100 <> 0) -> true
  |y when (y mod 400 = 0) -> true
  |y -> false;;

let is_y_bis = is_bisextil 2028;;
