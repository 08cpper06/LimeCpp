int sum(int lhs, int rhs)
{
	return lhs + rhs;
}

int main()
{
	int sum_val = 0;
	for (int i = 0; i < 10; ++i)
	{
		sum_val = sum(i, sum_val);
	}
	return sum_val;
}