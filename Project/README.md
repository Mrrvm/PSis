# PSis Project
<p>This is the implemented architcture for the <a href="https://github.com/Mrrvm/PSis/blob/master/Project_statement.pdf">assignment</a>:
</p>

<p align="center">
  <img src=".images/arch.png">
</p>



<hr>

<p>Compile gateway</p>

	gcc -g -pthread defs.c generic_list.c gateway/* -o [program]

<p>Compile peer</p>
	
	gcc -g -pthread defs.c generic_list.c peers/* -o [program]

<p>Compile cliente</p>
	
	gcc -g client/* -o [program]

<p>Execution order: gateway, peer (...), client (...)</p>
