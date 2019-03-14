Cat Stack {
    int[] stack
    int size
}

Cfn Stack {
    fun is_empty
    <- void
    -> bool
    {
        dec bool ret_val = true
        ret_val = | 0 == own.size |
        return ret_val
    }

    fun push
    <- void
    -> void
    {
        list[0] = 10
    }
}

start {
    dec list l <[ int ]> new [ 1, 2, 3]
    dec Stack s = new Stack { stack: l, size: 10 }
    dec bool c = s...is_empty()
}

