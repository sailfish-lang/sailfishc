start {
    // ok grouping
    if | 10 == 12 | {}
    else {}

    // illegal grouping
    if | 10 - 12 | {}
    else {}

    // illegal grouping
    if | !true | {}
    else {}

    // illegal grouping
    if |true| {}
    else {}
}