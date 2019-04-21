Uat {
    stack next
    int data
}

Ufn{
   (fun hasNext(void)(bool) {
       Tree ( (| own.next == empty | { return false } ))
       return true
   })

   (fun append(stack s)(void) {
       own.next = s
   })
}