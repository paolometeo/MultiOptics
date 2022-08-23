/*  
 *   From a float number to the near  integer 
*/
int Nint(float x) {
  int ciccio = 0;
  if(x >= 0) {
   ciccio = int(x + 0.5); 
}
  if(x < 0){ 
  ciccio = int(x - 0.5);
}
return ciccio;
}
