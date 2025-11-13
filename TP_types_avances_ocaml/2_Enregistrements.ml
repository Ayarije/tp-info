type complex = {
    re: float;
    im: float;
  };;

let make_complex x y = {re = x; im = y};;
let sum_complex z1 z2 = {re = z1.re +. z2.re; im = z1.im +. z2.im};;
let sq_module_cplx z = z.re**2. +. z.im**2.;;


let z1 = make_complex 5. 3.;;
let z2 = make_complex 10. 1.;;

let z_sum = sum_complex z1 z2;;
let z_module = sq_module_cplx z1;;
