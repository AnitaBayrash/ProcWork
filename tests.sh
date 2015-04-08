make >> logger.txt
sudo insmod ./proc.ko
echo "10*10 test:"
echo 10 > /proc/first_operand_proc
echo 10 > /proc/second_operand_proc
echo '*' > /proc/operation_proc
sudo cat /proc/result_proc
echo "20+30 test:"
echo 20 > /proc/first_operand_proc
echo 30 > /proc/second_operand_proc
echo '+' > /proc/operation_proc
sudo cat /proc/result_proc
echo "50-30 test:"
echo 50 > /proc/first_operand_proc
echo 30 > /proc/second_operand_proc
echo '-' > /proc/operation_proc
sudo cat /proc/result_proc
echo "81/9 test:"
echo 81 > /proc/first_operand_proc
echo 9 > /proc/second_operand_proc
echo '/' > /proc/operation_proc
sudo cat /proc/result_proc
sudo rmmod ./proc.ko
make clean >> logger.txt
