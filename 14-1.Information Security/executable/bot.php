<?php
// index.html���� answer�� �޾ƿ´� -> ��, ���ڿ��� �޾ƿ´�
$answer = $_POST["ANSWER"];

// PHP ������ str_split �Լ��� �������� ���� ���
if(!function_exists('str_split'))
{
	function str_split($string,$split_length=1)
	{
		$count = strlen($answer);
        
		if($split_length < 1)
		{
			return false;   
        }
		elseif($split_length > $count)
		{
			return array($strArr);  
        }
		else
		{
			$num = (int)ceil($count/$split_length);

			$strArr = array();

			for($i = 0; $i < $num ; $i++)
			{
				$strArr[] = substr($answer,$i*$split_length,$split_length);   
            }
			return $strArr;  
		}
	}   
}

// strArr �迭�� answer�� �����Ѵ�
$strArr = str_split($answer);

// �迭�� ũ�� ���ϱ� ���� �迭�� �ִ��� Ȯ��
if (!isset($strArr)) 
{
	// ������ ����
	echo "ERROR >> The array doesn't exist.";
	return -1;
}

$bot = false;

for ($i = 0; $i < count($strArr); $i++)
{
	if ($strArr[$i] == '.')		
		$bot = true;
}

if (count($strArr) > 2)
{
	$bot = $true;
}

$tmp=1;
$cnt=0;
for($i=count($strArr)-1;$i>=0;$i--){
	$cnt+=$tmp*($strArr[$i]-'0');
	$tmp*=10;
}
if ($bot == false)
{
	if ($cnt == 3)
		echo "True!";
	else
		echo "Input False!";
}
else 
{
	echo "False!";
}

return 0;
?>