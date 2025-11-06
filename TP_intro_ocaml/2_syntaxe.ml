
let a = 5 - 7;;
let b = 12 * 9;;

let biggest_val = if a >= b then
                    a
                  else
                    b;;

let smallest_val = 
    let a = 1. /. 4. +. 1. /. 8. in
    let b = 1. /. 3. +. 1. /. 12. in
    let c = 1. /. 5. +. 1. /. 6. in
    if a < c && a < b then
      a
    else if b < c then
      b
    else
      c;;

let mul = fun x -> fun y -> y*x;;
let r = mul 2 3;;

let add = fun x -> fun y -> x + y;;
let r = add 5 9;;

let mul x y = x * y;;
let mul_r = mul 2 3;;

let add x y = x + y;;
let add_r = add 5 9;;

let f_min a b = if a <= b then
                  a
                else
                  b;;

let f_min = fun a -> fun b -> if a <= b then
                                a
                              else
                                b;;
let min_r = f_min add_r mul_r;;

let fac n = let rec fac_step r n =
              if n > 0 then
                fac_step (r*n) (n-1)
              else
                r
            in
            fac_step 1 n;;

let factorial = fac 5;;

let power a n =
  let t = n in
  let k = a in
  let r = 1 in
  let rec power_step r k t =
    if t <= 0 then
      r
    else if t mod 2 = 0 then
      power_step r (k*k) (t/2)
    else
      power_step (r*k) k (t-1)
  in
  power_step r k t;;

let p = power 2 5;;

let int_to_float = float_of_int 5;;
let float_to_int = int_of_float int_to_float;;

let float_to_int = int_of_float 3.14;;
let int_to_float = float_of_int float_to_int;;
