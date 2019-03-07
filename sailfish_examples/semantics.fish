start {
    // +
    f = 10.0 + 10.0 // ok
    f = 1 + 2 // ok
    f = false + true // error
    f = 10 + 10.0 // error
    f = 10.0 + 10 // error

    // -
    f = 10.0 - 10.0 // ok
    f = 1 - 2 // ok
    f = false - true // error
    f = 10 - 10.0 // error
    f = 10.0 - 10 // error

    // *
    f = 10.0 * 10.0 // ok
    f = 1 * 2 // ok
    f = false * true // error
    f = 10 * 10.0 // error
    f = 10.0 * 10 // error

    // /
    f = 10.0 / 10.0 // ok
    f = 1 / 2 // ok
    f = false / true // error
    f = 10 / 10.0 // error
    f = 10.0 / 10 // error

    // **
    f = 10 ** 10 // ok
    f = 1 ** 2.0 // error
    f = false ** 1 // error

    // %
    f = 10 % 10 // ok
    f = 1 % 2.0 // error
    f = false % 1 // error

    // > comp
    f = 10.0 > 10.0 // ok
    f = 1 > 2 // ok
    f = false > true // error
    f = 10 > 10.0 // error
    f = 10.0 > 10 // error

    // >= comp
    f = 10.0 >= 10.0 // ok
    f = 1 >= 2 // ok
    f = false >= true // error
    f = 10 >= 10.0 // error
    f = 10.0 >= 10 // error

    // < comp
    f = 10.0 < 10.0 // ok
    f = 1 < 2 // ok
    f = false < true // error
    f = 10 < 10.0 // error
    f = 10.0 < 10 // error

    // <= comp
    f = 10.0 <= 10.0 //ok
    f = 1 <= 2 // ok
    f = false <= true // error
    f = 10 <= 10.0 // error
    f = 10.0 <= 10 // error

    // equivalence comp
    f = false == true // ok
    f = true == 10 // error
    f = 10 == false // error

    // nonequivalence comp
    f = false != true // ok
    f = true != 10 // error
    f = 10 != false // error

    // and comp
    f = false and true // ok
    f = true and 10 // error
    f = 10 and false // error
    
    // or comp
    f = false or true // ok
    f = true or 10 // error
    f = 10 or false // error

    // negation
    f = !true // ok
    f = !10 // error
}