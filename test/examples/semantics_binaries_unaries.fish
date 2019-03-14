start {
    dec int i = 1
    dec flt f = 10.0
    dec bool b = true

    // +
    f = 10.0 + 10.0 // ok
    i = 1 + 2 // ok
    b = false + true // error
    i = 10 + 10.0 // error
    f = 10.0 + 10 // error

    // -
    f = 10.0 - 10.0 // ok
    i = 1 - 2 // ok
    b = false - true // error
    i = 10 - 10.0 // error
    f = 10.0 - 10 // error

    // *
    f = 10.0 * 10.0 // ok
    i = 1 * 2 // ok
    b = false * true // error
    i = 10 * 10.0 // error
    f = 10.0 * 10 // error

    // /
    f = 10.0 / 10.0 // ok
    i = 1 / 2 // ok
    b = false / true // error
    i = 10 / 10.0 // error
    f = 10.0 / 10 // error

    // > comp
    f = 10.0 > 10.0 // ok
    i = 1 > 2 // ok
    b = false > true // error
    i = 10 > 10.0 // error
    f = 10.0 > 10 // error

    // >= comp
    f = 10.0 >= 10.0 // ok
    i = 1 >= 2 // ok
    b = false >= true // error
    i = 10 >= 10.0 // error
    f = 10.0 >= 10 // error

    // < comp
    f = 10.0 < 10.0 // ok
    i = 1 < 2 // ok
    b = false < true // error
    i = 10 < 10.0 // error
    f = 10.0 < 10 // error

    // <= comp
    f = 10.0 <= 10.0 //ok
    i = 1 <= 2 // ok
    b = false <= true // error
    i = 10 <= 10.0 // error
    f = 10.0 <= 10 // error

    // **
    i = 10 ** 10 // ok
    i = 1 ** 2.0 // error
    b = false ** 1 // error
   
    // %
    i = 10 % 10 // ok
    i = 1 % 2.0 // error
    b = false % 1 // error

    // equivalence comp
    b = false == true // ok
    b = true == 10 // error
    i = 10 == false // error

    // nonequivalence comp
    b = false != true // ok
    b = true != 10 // error
    i = 10 != false // error

    // and comp
    b = false and true // ok
    b = true and 10 // error
    i = 10 and false // error
    
    // or comp
    b = false or true // ok
    b = true or 10 // error
    i = 10 or false // error

    // negation
    b = !true // ok
    b = !10 // error
}