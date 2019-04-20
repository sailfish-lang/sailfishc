import intListHandler : "../examples/intListHandler.fish"

start {
   dec [int] list = [1,2,3]
   dec int size = 3
   dec intListHandler bar = new intListHandler { list: list, size: size}

   bar...printFoo(void)
   bar...push(10)
   bar...printFoo(void)
   bar...removeByIndex(1)
   bar...printFoo(void)

   bar...setAtIndex(1, 22)
   bar...setAtIndex(20, 22)
   bar...printFoo(void)
}
