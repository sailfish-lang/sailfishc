// --------       dictionary        -------
dec dictionary dictionary_good <[ int : flt ]> new { 1 : 1.0 } // good
dec dictionary dictionary_error_a <[ Foo : flt ]> new { 1 : 1.0 } // error key type does not exist
dec dictionary dictionary_error_b <[ int : Foo ]> new { 1 : 1.0 } // error value type does not exist
dec dictionary str <[ int : flt ]> new { 1 : 1.0 } // error name is reserved
dec dictionary dictionary_error_c <[ int : flt ]> new [ 1 ] // error type is not a dictionary
dec dictionary dictionary_error_d <[ int : flt ]> new { 1 : 1.0, 1.0 : 1} // error dictionary is not homogenous
dec dictionary dictionary_error_e <[ int : flt ]> new { 1.0 : 1.0 } // error dictionary keys don't match
dec dictionary dictionary_error_f <[ int : flt ]> new { 1 : 1 } // error dictionary values don't match

start {}