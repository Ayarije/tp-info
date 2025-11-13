let euclidean_div a b = (a/b, a mod b);;

let add_q_r =
  let (q, r) = euclidean_div 9 5 in
  q + r;;
