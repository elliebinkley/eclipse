#! /bin/bash
declare -i max=0;
declare -i length=0;
while read line 
do
   line1="$(echo -e "${line}" | sed -e 's/[[:space:]]*$//')"
   length=${#line1}
#   echo $length $line
   if (( max < length ))
   then
	max=length;
	echo $max $line1;
    fi
done < words_alpha.txt

    
    
  
    
	    
