namespace test
{
	class Program
	{
		static float ApplyFilter(float[] image, float[] filterMaks, int maskSize, int x, int y, int xRes)
		{
			int halfMaskSize = maskSize / 2;
			float result = 0;
			for (int i = 0; i < maskSize; i++)
			{
				for (int j = 0; j < maskSize; j++)
				{
					result += image[(x + i) * xRes + y + j] * filterMaks[i * maskSize + j];
				}
			}

			return result;
		}

		static void Main(string[] args)
		{
			//int xRes = 4;
			//int yRes = 3;
			//int enlargeBy = 1;
			//
			//int[] orig = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
			//int[] enlarged = new int[enlargedxRes * (yRes + 2 * enlargeBy)];

			int xRes = 3;
			int yRes = 2;
			int enlargeBy = 1;
			int enlargedxRes = xRes + 2 * enlargeBy;

			float[] orig = { 1, 2, 3, 4, 5, 6 };
			float[] enlarged = new float[enlargedxRes * (yRes + 2 * enlargeBy)];



			for (int i = 0; i < yRes + 2 * enlargeBy; i++)
			{
				for (int j = 0; j < enlargedxRes; j++)
				{
					if (j < enlargeBy || i < enlargeBy || i >= yRes + enlargeBy || j >= xRes + enlargeBy)
					{
						enlarged[i * enlargedxRes + j] = 0;
					}
					else
					{
						enlarged[i * enlargedxRes + j] = orig[(i-enlargeBy) * (xRes) + j - enlargeBy];

					}
				}
			}

			float[] filterMask = { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f };
			for (int i = 0; i < yRes; i++)
			{
				for (int j = 0; j < xRes; j++)
				{
					orig[(i * xRes + j)] = ApplyFilter(enlarged, filterMask, 3, i, j, enlargedxRes);
				}
			}
		}
	}
}
