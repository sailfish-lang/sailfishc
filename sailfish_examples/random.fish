Cat Foo {
	int i	
}

Cfn Foo {
    	
	fun foo
    	<- int i
    	-> flt
    	{}
}

start {
	dec int i = 10
	dec Foo foo = new Foo { i: 10 }
	i = foo.i + i
}
