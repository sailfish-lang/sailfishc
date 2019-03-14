Cat Stack {
    int size
}

Cfn Stack {
    fun is_empty
    <- void
    -> bool
    {
        dec bool ret_val = true
        ret_val = | false == true |
        return ret_val
    }
}

start {
    dec Stack s = new Stack { size: 10 }
    dec bool c = s...is_empty()
}

