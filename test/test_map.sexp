(program

(= list_equal (lambda (x y)
	(if (|| (is_nil x) (is_nil y))
		(&& (is_nil x) (is_nil y))
		(&& (==			(car x) (car y))
			(list_equal (cdr x) (cdr y))
		)
	)
))

(= map (lambda (fun list)
	(if (is_nil list)
		(quote ())
		(cons (fun (car list)) (map fun (cdr list)))
	)
))

(= add_4 (lambda (x)
	(+ x 4)
))


(= in_place_map (map (lambda (x) (+ 4 x)) (quote (3 2 1))))
(= fun_call_map (map add_4 (quote (3 2 1))))

(&& (list_equal in_place_map fun_call_map)
	(list_equal fun_call_map (quote (7 6 5)))
)

)
