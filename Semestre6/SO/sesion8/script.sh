hora=`date +%H`
if test $hora -lt 12 
then
	echo "Buenos días, $LOGNAME"
else 
	if test $hora -lt 17
	then
		echo "Buenas tardes, $LOGNAME"
	else
		echo "Buenas noches, $LOGNAME"
	fi
fi