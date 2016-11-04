<html>
 <head>
  <title>Calculator</title>
 </head>
 <body>
   <h1>Calculator</h1>
   By Meghana Ginjpalli <br><br>
   Type an expression in the following box (e.g., 10.5+20*3/25). <br><br>
   <form action='<?php echo $_SERVER['PHP_SELF'];?>' method='GET'>
       <input type='text' name='expr'>
       <input type='submit' value='Calculate'>
   </form>

   <ul>
     <li>Only numbers and +,-,* and / operators are allowed in the expression.
     <li>The evaluation follows the standard operator precedence.
     <li>The calculator does not support parentheses.
     <li>The calculator handles invalid input "gracefully". It does not output PHP error messages.
   </ul>
   Here are some (but not limit to) reasonable test cases:
   <ol>
     <li> A basic arithmetic operation: 3+4*5=23 </li>
     <li> An expression with floating point or negative sign: -3.2+2*4-1/3 = 4.46666666667, 3+-2.1*2 = -1.2 </li>
     <li> Some typos inside operation (e.g. alphabetic letter): Invalid input expression 2d4+1 </li>
  </ol>

   <?php 
     $expr=$_GET["expr"];

     if (preg_match("/\/[0]/",$expr)) {
      // if there is a divide by 0 case, throw no exception
     }
     // if it is a valid expression, then display the answer
     elseif (preg_match("/^[-+*.\/, 0-9]+$/",$expr)) {
      eval("\$answer=$expr;");
      echo $expr . " = " . $answer;
     }
     // if it is an invalid expression, indicate as so
     else {
      echo "Invalid Expression";
     }
   ?> 
 </body>
</html>
