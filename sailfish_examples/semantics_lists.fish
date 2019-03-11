// ---------        list         ---------
dec list list_good <[ int ]> new [ 1 ] // good
dec list list_error_a <[ Foo ]> new [ 1 ] // error key type does not exist
dec list list_error_b <[ int ]> new { 1 : 1.0 }  // error type is not a list
dec list list_error_c <[ int ]> new [ 1, 1.0 ]  // error list is not homogenous
dec list list_error_d <[ int ]> new [ 1.0, 1.0 ]  // error list is not same as declared