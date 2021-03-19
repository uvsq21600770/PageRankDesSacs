#include <QuickSort.h>
    
void swap(double *x, double *y)
{
   double tmp;
   tmp = *x;
   *x = *y;
   *y = tmp;
}

int pivot(int i, int j)
{
   return ((i+j)/2); 
}

void quickSort(double values[], int low, int high)
{
   int start;
   int end;
   double k;
   int p;

  if (low < high)
  {
     p = pivot(low,high);
     swap(&values[low],&values[p]);
     k = values[low];
     start = (low+1);
     end = high;

     while (start <= end)
     {
        while ((start <= high) && (values[start] <= k))
        {
           start++;
        }
        while ((end >= low) && (values[end] > k))
        { 
           end--;
        }
        if (start < end)
        {
           swap(&values[start],&values[end]);
        }
     }
     swap(&values[low],&values[end]);
     quickSort(values,low,(end-1));
     quickSort(values,(end+1),high);
  }
}