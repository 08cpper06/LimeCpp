int sum(int lhs, int rhs)
{
	return lhs + rhs;
}

int main()
{
	int arr[10];
	for (int i = 10; i >= 0; i--)
	{
		arr[i] = sum(i, 2);
	}
	return 0;
}