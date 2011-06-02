(program 

(= list_equal (lambda (x y)
	(if (|| (is_nil x) (is_nil y))
		(&& (is_nil x) (is_nil y))
		(&& (==			(car x) (car y))
			(list_equal (cdr x) (cdr y))
		)
	)
))

(= put_at_end_of (lambda (list elem) 
	(if (is_nil list)
		(cons elem (quote ()))
		(cons (car list) (put_at_end_of (cdr list) elem))	
	)
))

(= reverse (lambda (m)
	(if (is_nil m)
		(quote ())
		(put_at_end_of (reverse (cdr m)) (car m))	
	)
))

(&& (&&
	(list_equal (reverse (quote ())) (quote ()))
	(list_equal (reverse (quote (3))) (quote (3)))
	)
	(list_equal (reverse (quote (1 2 3))) (quote (3 2 1)))
)

)

