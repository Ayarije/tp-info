let range n =
  let arr = Array.make n 0 in
  let rec loop i =
    match i with
    | i when i < 0 -> ()
    | i ->
       arr.(i) <- i;
       loop (i - 1)
  in
  loop (n-1);
  arr;;

let a = range 100;;

let array_init n constructor =
  let arr = Array.make n 0 in
  for i = 0 to n-1 do
    arr.(i) <- constructor i
  done;
  arr;;

let a2 = array_init 5 (fun x -> x*x);;

let array_iter arr f =
  let n = Array.length arr in
  for i = 0 to n-1 do
    arr.(i) <- f arr.(i)
  done;;

array_iter a (fun x -> x*x);;

let a3 = Array.copy a;;

let print_float_array arr =
  let n = Array.length arr in
  Printf.printf "[|";
  for i = 0 to n-1 do
    Printf.printf "%f" arr.(i);
    if i <> n-1 then
      Printf.printf "; "
    else
      Printf.printf "|]"
  done;;

let a4 = Array.make 8 2.0;;
print_float_array a4;;

let matrix_copy mat =
  let h = Array.length mat in
  let w = Array.length mat.(0) in
  let cpy_mat = Array.make_matrix w h 0 in
  for i = 0 to h-1 do
    cpy_mat.(i) <- Array.copy mat.(i)
  done;
  cpy_mat;;

let m1 = Array.make_matrix 5 5 0;;
let m2 = matrix_copy m1;;

let find_fib_index n =
  let u0 = ref 0 in
  let u1 = ref 1 in
  let cursor = ref 0 in
  while !u1 < n && !u0 < n do
    if !cursor mod 2 = 0 then begin
        u0 := !u0 + !u1;
        cursor := !cursor + 1
      end
    else begin
        u1 := !u0 + !u1;
        cursor := !cursor + 1
      end;
  done;
  !cursor + 1;;

let fib_i = find_fib_index 34542155812386;;
