exception NotExhaustiveValuation;;
type formula =
  | Var of int
  | Top
  | Bot
  | Not of formula
  | Or of formula * formula
  | And of formula * formula
  | Imply of formula * formula;;

let f1 = Imply(Var(4), Imply(Imply(Var(3), Var(2)), Var(1)));;
let f2 = Or(And(Var(1), Imply(Var(2), Var(1))), Bot);;

let rec formula_height formule =
  match formule with
  | Var(id)       -> 0
  | Top           -> 0
  | Bot           -> 0
  | Not(f)        -> 1 + (formula_height f)
  | Or(f1, f2)    -> 1 + (max (formula_height f1) (formula_height f2))
  | And(f1, f2)   -> 1 + (max (formula_height f1) (formula_height f2))
  | Imply(f1, f2) -> 1 + (max (formula_height f1) (formula_height f2));;

let f1_h = formula_height f1;;
let f2_h = formula_height f2;;

let rec formula_size formule =
  match formule with
  | Var(id)       -> 1
  | Top           -> 1
  | Bot           -> 1
  | Not(f)        -> 1 + (formula_size f)
  | Or(f1, f2)    -> 1 + (formula_size f1) + (formula_size f2)
  | And(f1, f2)   -> 1 + (formula_size f1) + (formula_size f2)
  | Imply(f1, f2) -> 1 + (formula_size f1) + (formula_size f2);;

let f1_size = formula_size f1;;
let f2_size = formula_size f2;;

let rec get_value id valuation =
  match valuation with
  | [] -> raise NotExhaustiveValuation
  | (i, v)::tail when i = id -> v
  | hd::tail -> get_value id tail;;

let rec evaluate valuation formule =
  match formule with
  | Var(id)       -> get_value id valuation
  | Top           -> true
  | Bot           -> false
  | Not(f)        -> not (evaluate valuation f)
  | Or(f1, f2)    -> (evaluate valuation f1) || (evaluate valuation f2)
  | And(f1, f2)   -> (evaluate valuation f1) && (evaluate valuation f2)
  | Imply(f1, f2) -> not (evaluate valuation f1) || (evaluate valuation f2);;

let vals = [(1, true); (2, false); (3, false)];;

let f1_v = evaluate vals f1;;
let f2_v = evaluate vals f2;;

let rec uni_concat lst1 lst2 =
  let add e lst =
    if List.mem e lst then lst
    else                   e::lst
  in
  match lst1, lst2 with
  | [], []    -> []
  | v::tl, _  -> add v (uni_concat tl lst2)
  | [], v::tl -> add v (uni_concat lst1 tl)

let rec variable_list formula =
  match formula with
  | Var(id)       -> [id]
  | Top           -> []
  | Bot           -> []
  | Not(f)        -> variable_list f
  | Or(f1, f2)    -> uni_concat (variable_list f1) (variable_list f2)
  | And(f1, f2)   -> uni_concat (variable_list f1) (variable_list f2)
  | Imply(f1, f2) -> uni_concat (variable_list f1) (variable_list f2);;

let f1_var_lst = variable_list f1;;
let f2_var_lst = variable_list f2;;

let rec fast_exp a n =
  match a, n with
  | a, 0 -> 1
  | a, k when k mod 2 = 0 -> fast_exp (a*a) (k/2)
  | a, k -> a*fast_exp a (k-1);;

let rec generate_all_valuations var_lst incr =
  let rec bin_of_int n len =
    match n, len with
    | _, 0 -> []
    | n, l -> (if n mod 2 = 0 then false else true)::(bin_of_int (n/2) (len - 1))
  in
  let rec gen_val var_lst incr =
    match var_lst, incr with
    | [], []                  -> []
    | var::var_tl, v::incr_tl -> (var, v)::(gen_val var_tl incr_tl)
  in
  let len = List.length var_lst in
  let threshold = fast_exp 2 len in
  match incr with
  | i when i >= threshold -> []
  | i -> (gen_val var_lst (bin_of_int i len))::(generate_all_valuations var_lst (i + 1));;

let f1_vals = generate_all_valuations f1_var_lst 0;;
let f2_vals = generate_all_valuations f2_var_lst 0;;

let is_satisfiable formula =
  let f_vals = generate_all_valuations (variable_list formula) 0 in
  let rec go_through_vals vals =
    match vals with
    | [] -> false
    | valuation::tail ->
       if evaluate valuation formula then
         true
       else
         go_through_vals tail
  in
  go_through_vals f_vals;;

let is_f1_sat = is_satisfiable f1;;
let is_f2_sat = is_satisfiable f2;;
let is_Top_sat = is_satisfiable Top;;
let is_Bot_sat = is_satisfiable Bot;;

let is_contradiction formula = not (is_satisfiable formula);;

let is_f1_ant = is_contradiction f1;;
let is_f2_ant = is_contradiction f2;;
let is_Top_ant = is_contradiction Top;;
let is_Bot_ant = is_contradiction Bot;;

let rec subformulas formula =
  match formula with
  | Var(id)        -> [Var(id)]
  | Top            -> [Top]
  | Bot            -> [Bot]
  | Not(f)         -> Not(f)::(subformulas f)
  | And(f1, f2)    -> (And(f1, f2)::(subformulas f1))@(subformulas f2)
  | Or(f1, f2)     -> (Or(f1, f2)::(subformulas f1))@(subformulas f2)
  | Imply(f1, f2)  -> (Imply(f1, f2)::(subformulas f1))@(subformulas f2);;

let f1_sub = subformulas f1;;
let f2_sub = subformulas f2;;

let rec simplify formula =
  match formula with
  | Var(id) -> Var(id)
  | Top     -> Top
  | Bot     -> Bot
  | Not(f)  ->
     (match f with
     | Top    -> Bot
     | Bot    -> Top
     | f      -> simplify f)
  | And(f1, f2) ->
     (match f1, f2 with
     | f, Top -> simplify f
     | Top, f -> simplify f
     | f, Bot -> Bot
     | Bot, f -> Bot
     | f1, f2 -> And(simplify f1, simplify f2))
  | Or(f1, f2) ->
     (match f1, f2 with
     | f, Top -> Top
     | Top, f -> Top
     | f, Bot -> simplify f
     | Bot, f -> simplify f
     | f1, f2 -> Or(simplify f1, simplify f2))
   | Imply(f1, f2) ->
     (match f1, f2 with
     | Top, f -> simplify f
     | Bot, f -> Top
     | f1, f2 -> Imply(simplify f1, simplify f2));;

let f1_simple = simplify f1;;
let f2_simple = simplify f2;;

let is_tautology formula =
  let f_vals = generate_all_valuations (variable_list formula) 0 in
  let rec go_through_vals vals =
    match vals with
    | [] -> true
    | valuation::tail ->
       if evaluate valuation formula then
         go_through_vals tail
       else
         false
  in
  go_through_vals f_vals;;

let f1_tau = is_tautology f1;;
let f2_tau = is_tautology f2;;
let top_tau = is_tautology Top;;
let bot_tau = is_tautology Bot;;

let are_equivalent f1 f2 =
  let rec go_through_vals f1_vals f2_vals =
    match f1_vals, f2_vals with
    | [], [] -> true
    | val1::tl1, val2::tl2 ->
       if evaluate val1 f1 = evaluate val2 f2 then
         go_through_vals tl1 tl2
       else
         false
  in
  let f1_var_list = variable_list f1 in
  let f2_var_list = variable_list f2 in
  if List.length f1_var_list <> List.length f2_var_list then
    false
  else
    let f1_vals = generate_all_valuations f1_var_list 0 in
    let f2_vals = generate_all_valuations f2_var_list 0 in
    go_through_vals f1_vals f2_vals;;

let is_f1_f2_eq = are_equivalent f1 f2;;

let rec substitute old_f var sub_f =
  let sub v old_f = substitute old_f v sub_f in
  match old_f with
  | Var(id)        -> if var = id then sub_f else Var(id)
  | Top            -> Top
  | Bot            -> Bot
  | Not(f)         -> Not(sub var f)
  | And(f1, f2)    -> And(sub var f1, sub var f2)
  | Or(f1, f2)     -> Or(sub var f1, sub var f2)
  | Imply(f1, f2)  -> Imply(sub var f1, sub var f2)

let substituted_f1 = substitute f1 4 Bot;;

let rec sprint_top_line var_list =
  match var_list with
  | [] -> "| f |"
  | var::tl ->
     (Printf.sprintf "| %d " var) ^ (sprint_top_line tl);;

let rec sprint_sep var_list =
  match var_list with
  | [] -> "----"
  | var::tl -> "----" ^ (sprint_sep tl);;

let rec sprint_line v f val_f =
  match v with
  | [] -> Printf.sprintf "| %d |" (if val_f then 1 else 0)
  | (var, v_val)::tl ->
     (Printf.sprintf "| %d " (if v_val then 1 else 0)) ^ (sprint_line tl f val_f);;

let rec sprint_truth_lines vals f =
  match vals with
  | [] -> ""
  | v::tl ->
     (sprint_line v f (evaluate v f)) ^ "\n" ^ sprint_truth_lines tl f;;

let print_truth_table formula =
  let f_vars = variable_list formula in
  let f_vals = generate_all_valuations f_vars 0 in
  let top_line = sprint_top_line f_vars in
  let sep_line = sprint_sep f_vars in
  let truth_table = sprint_truth_lines f_vals formula in
  print_string (top_line ^ "\n" ^ sep_line ^ "\n" ^ truth_table);;

let f3 = Or(Or(Imply(Var(6), Var(5)), And(Var(3), Or(Var(4), Var(2)))), And(Var(1), Var(5)));;

let _ = print_truth_table f1;;
let _ = print_truth_table f2;;
let _ = print_truth_table f3;;
