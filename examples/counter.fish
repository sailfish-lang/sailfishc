Cat Counter {
    int count
    str finish_msg
}

Cfn Counter {
    fun decrement
    <- void
    -> void
    {
        own.count = own.count - 1
        display_int(own.count)
        display_str("\n")
    }

    fun increment
    <- void
    -> void
    {
        own.count = own.count + 1
        display_int(own.count)
        display_str("\n")
    }

    fun countdown
    <- void
    -> void
    {
        if | own.count > 0 | { 
            own...decrement()
            own...countdown()
        }

        else {
            display_str( own.finish_msg )
        }
    }
}

start {
    dec Counter c = new Counter { count : 10, finish_msg: "Blast off!\n" }

    c...countdown()
}