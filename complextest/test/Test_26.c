int main()
{
	int sum_val = 0;
	for (int i = 0; i < 10; i = i + 1)
	{
		sum_val = sum(i, sum_val);
	}
	return sum_val;
}