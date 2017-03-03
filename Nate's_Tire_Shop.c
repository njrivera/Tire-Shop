/* *******************************************************
  *																																						*
  *																Nate's Tire Shop														*
  *																																						*
  *********************************************************/

#include<stdio.h>
#include<string.h>

#define MAX_CHARS 35
#define MAX_TIRES 100

typedef struct
{
	int width, aspect_ratio, rim;
} Size;

typedef struct
{
	char name[MAX_CHARS+1];
	Size tire_size;
	float price;
	int on_hand, item_num;
} Tire;

void quick_sort(Tire *tire_array, int tire_count, int sort_by);
void sell_tire(Tire *tire_array, int tire_count);
void stock_tire(Tire *tire_array, int tire_count);
void search_tire(Tire *tire_array, int tire_count);
void fill_inv(Tire *tire_array, int tire_count);
void fill_warehouse(Tire *tire_array, int tire_count);
Size convert(int height, float width, int rim);
int round_width(float x);
int round_aspect_ratio(float x);
void print_tires (Tire *tire_array, int tire_count);
int insert_tire(Tire *tire_array, int tire_count);
int delete_tire(Tire *tire_array, int tire_count);

int main(void)
{
	FILE *warehouse = fopen("Tire_Warehouse.txt", "r");
	int answer = -1, tire_count = 0, ch, last_ch = 0;
	Tire tire_array[MAX_TIRES];
	char *string[MAX_CHARS];
	
	while((ch = fgetc(warehouse)) != EOF)
	{
		if(ch == '\n')
			tire_count++;
	}
	
	fill_inv(tire_array, tire_count);
	
	printf("\n*****************************************\n*\t\t\t\t\t*\n*     ");
	printf("Welcome to Nate's Tire Shop!\t*\n*\t\t\t\t\t*\n*****************************************\n");
	while(answer != 0)
	{
		printf("\nChoose from the following options:\n\n");
		printf("1 - Show tires in stock\n");
		printf("2 - Stock\n");
		printf("3 - Sell\n");
		printf("4 - Add new tire\n");
		printf("5 - Delete tire\n");
		printf("0 - Quit\n\n");
		scanf("%d", &answer);
	
		switch(answer)
		{
			case 1: print_tires(tire_array, tire_count);
			break;
			case 2: stock_tire(tire_array, tire_count);
			break;
			case 3: sell_tire(tire_array, tire_count);
			break;
			case 4: tire_count = insert_tire(tire_array, tire_count);
			break;
			case 5: tire_count = delete_tire(tire_array, tire_count);
			break;
			case 0:
			break;
			default: printf("Invalid Input\n");
			break;
		}
	}
	fill_warehouse(tire_array, tire_count);
	fclose(warehouse);
	return 0;
}

void stock_tire(Tire *tire_array, int tire_count)
{
	int temp_item, stock_qty;
	search_tire(tire_array, tire_count);
	printf("\nChoose item number: ");
	scanf("%d", &temp_item);
	printf("How many: ");
	scanf("%d", &stock_qty);
	tire_array[temp_item].on_hand += stock_qty;
}

void sell_tire(Tire *tire_array, int tire_count)
{
	int temp_item, sell_qty;
	search_tire(tire_array, tire_count);
	printf("\nChoose item number: ");
	scanf("%d", &temp_item);
	printf("How many: ");
	scanf("%d", &sell_qty);
	tire_array[temp_item].on_hand -= sell_qty;
}

void search_tire(Tire *tire_array, int tire_count)
{
	int i = 0;
	Tire x;
	printf("Width: ");
	scanf("%d", &x.tire_size.width);
	printf("Aspect Ratio: ");
	scanf("%d", &x.tire_size.aspect_ratio);
	printf("Rim Size: ");
	scanf("%d", &x.tire_size.rim);
	printf("\nItem#   Size\t\tName\t\t\t\t   Price       OnHnd\n");
	printf("_____   ____\t\t____\t\t\t\t   _____       _____\n\n");
	for(; i < tire_count; i++)
	{
		if(x.tire_size.width == tire_array[i].tire_size.width
		&& x.tire_size.aspect_ratio == tire_array[i].tire_size.aspect_ratio
		&& x.tire_size.rim == tire_array[i].tire_size.rim)
		{
			printf("%3d\t%d/%dR%d", tire_array[i].item_num,
									tire_array[i].tire_size.width, 
									tire_array[i].tire_size.aspect_ratio, 
									tire_array[i].tire_size.rim);
			printf("\t%-35s$%-10.2f %d\n", tire_array[i].name, tire_array[i].price, tire_array[i].on_hand);
		}
	}
}

void fill_inv(Tire *tire_array, int tire_count)
{
	int i = 0;
	FILE *warehouse = fopen("Tire_Warehouse.txt", "r");
	for(; i<tire_count; i++)
	{
		fscanf(warehouse, "%d", &tire_array[i].item_num);
		fscanf(warehouse, "%d/%dR%d", &tire_array[i].tire_size.width, 
									&tire_array[i].tire_size.aspect_ratio, 
									&tire_array[i].tire_size.rim);
		fgetc(warehouse);
		fgets(tire_array[i].name, 35, warehouse);
		fgetc(warehouse);
		fscanf(warehouse, "$%f", &tire_array[i].price);
		fscanf(warehouse, "%d", &tire_array[i].on_hand);
	}
	fclose(warehouse);
}

void fill_warehouse(Tire *tire_array, int tire_count)
{
	int i = 0;
	FILE *warehouse = fopen("Tire_Warehouse.txt", "w");
	for(; i<tire_count;i++)
	{
		fprintf(warehouse, "%d\t%d/%dR%d", tire_array[i].item_num, 
											tire_array[i].tire_size.width, 
											tire_array[i].tire_size.aspect_ratio, 
											tire_array[i].tire_size.rim);
		fprintf(warehouse, "\t%-35s$%-10.2f %d\n", tire_array[i].name, tire_array[i].price, tire_array[i].on_hand);
	}
	fclose(warehouse);
}

void print_tires(Tire *tire_array, int tire_count)
{
	int i = 0, sort_by;
	printf("\nSort by:\n\n");
	printf("1 - Item number\n");
	printf("2 - Size\n");
	printf("3 - Name\n");
	printf("4 - Price\n");
	printf("5 - On hand\n\n");
	scanf("%d", &sort_by);
	if(sort_by < 1 || sort_by > 5)
	{
		printf("\nInvalid Input\n");
		return;
	}
	quick_sort(tire_array, tire_count, sort_by);
	
	printf("\nItem#   Size\t\tName\t\t\t\t   Price       OnHnd\n");
	printf("_____   ____\t\t____\t\t\t\t   _____       _____\n\n");
	for (; i < tire_count; i++)
	{	
		printf("%3d\t%d/%dR%d", tire_array[i].item_num, 
								tire_array[i].tire_size.width, 
								tire_array[i].tire_size.aspect_ratio, 
								tire_array[i].tire_size.rim);
		printf("\t%-35s$%-10.2f %d\n", tire_array[i].name, tire_array[i].price, tire_array[i].on_hand);
	}
	quick_sort(tire_array, tire_count, 1);
}

void quick_sort(Tire *tire_array, int tire_count, int sort_by)
{
	int i, j;
	
	Tire pivot, temp;
	
	if(tire_count < 2)
		return;
	
	pivot = tire_array[tire_count/2];
	
	switch(sort_by)
	{
		case 1:
		{
			for(i = 0, j  = tire_count-1;; i++, j--)
			{
				while(tire_array[i].item_num < pivot.item_num)
					i++;
				while(pivot.item_num < tire_array[j].item_num)
					j--;
				if(i >= j)
					break;
				temp = tire_array[i];
				tire_array[i] = tire_array[j];
				tire_array[j] = temp;	
			}
			quick_sort(tire_array, i, 1);
			quick_sort(tire_array + i, tire_count - i, 1);
		}
		break;
		case 2:
		{
			int tire_int[tire_count];
			int pivot_int = pivot.tire_size.rim * 100000 + pivot.tire_size.width * 100 + pivot.tire_size.aspect_ratio;
			int temp_int = temp.tire_size.rim * 100000 + temp.tire_size.width * 100 + temp.tire_size.aspect_ratio;			
			for(i = 0; i < tire_count; i++)
			{
				tire_int[i] = tire_array[i].tire_size.rim * 100000 + 
				tire_array[i].tire_size.width * 100 + tire_array[i].tire_size.aspect_ratio;
			}
			
			for(i = 0, j  = tire_count-1;; i++, j--)
			{
				while(tire_int[i] < pivot_int)
					i++;
				while(pivot_int < tire_int[j])
					j--;
				if(i >= j)
					break;
				temp = tire_array[i];
				temp_int = tire_int[i];
				tire_array[i] = tire_array[j];
				tire_int[i] = tire_int[j];
				tire_array[j] = temp;	
				tire_int[j] = temp_int;
			}
			quick_sort(tire_array, i, 1);
			quick_sort(tire_array + i, tire_count - i, 1);			
		}
		break;
		case 3:
		{
			for(i = 0, j  = tire_count-1;; i++, j--)
			{
				while(strcmp(tire_array[i].name, pivot.name) < 0)
					i++;
				while(strcmp(pivot.name, tire_array[j].name) < 0)
					j--;
				if(i >= j)
					break;
				temp = tire_array[i];
				tire_array[i] = tire_array[j];
				tire_array[j] = temp;	
			}
			quick_sort(tire_array, i, 3);
			quick_sort(tire_array + i, tire_count - i, 3);
		}
		case 4:
		{
			for(i = 0, j  = tire_count-1;; i++, j--)
			{
				while(tire_array[i].price < pivot.price)
					i++;
				while(pivot.price < tire_array[j].price)
					j--;
				if(i >= j)
					break;
				temp = tire_array[i];
				tire_array[i] = tire_array[j];
				tire_array[j] = temp;	
			}
			quick_sort(tire_array, i, 4);
			quick_sort(tire_array + i, tire_count - i, 4);
		}
		break;
		case 5:
		{
			for(i = 0, j  = tire_count-1;; i++, j--)
			{
				while(tire_array[i].on_hand < pivot.on_hand)
					i++;
				while(pivot.on_hand < tire_array[j].on_hand)
					j--;
				if(i >= j)
					break;
				temp = tire_array[i];
				tire_array[i] = tire_array[j];
				tire_array[j] = temp;	
			}
			quick_sort(tire_array, i, 5);
			quick_sort(tire_array + i, tire_count - i, 5);
		}
		break;
	}
}

int insert_tire(Tire *tire_array, int tire_count)
{
	int format = 0;
	printf("Metric or Inch?\n\n1 - Metric\n2 - Inch\n\n");
	scanf("%d", &format);
	switch (format)
	{
		case 1:
		{
			printf("Width: ");
			scanf("%d", &tire_array[tire_count].tire_size.width);
			printf("Aspect Ratio: ");
			scanf("%d", &tire_array[tire_count].tire_size.aspect_ratio);
			printf("Rim Size: ");
			scanf("%d", &tire_array[tire_count].tire_size.rim);
		}
		break;
		case 2:
		{
			int height, rim;
			float width;
			printf("Height: ");
			scanf("%d", &height);
			printf("Width: ");
			scanf("%f", &width);
			printf("Rim Size: ");
			scanf("%d", &rim);
			tire_array[tire_count].tire_size = convert(height, width, rim);
		}
		break;
		default:
		{
			printf("Invalid input\n");
			return tire_count;
		}
	}
	printf("Name: ");
	gets(tire_array[tire_count].name);
	gets(tire_array[tire_count].name);
	printf("Price: $");
	scanf("%f", &tire_array[tire_count].price);
	printf("How many tires would you like to insert? ");
	scanf("%d", &tire_array[tire_count].on_hand);
	tire_array[tire_count].item_num = tire_count;
	int i = 0, tire_item = -1;
	for(; i < tire_count - 1; i++)
	{
		if(tire_array[i].item_num !=  i)
		{
			tire_array[tire_count].item_num = i;
			break;
		}
	}
	tire_count++;
	quick_sort(tire_array, tire_count, 1);
	return tire_count;
}
 
int delete_tire(Tire *tire_array, int tire_count)
 {
	printf("Which Tire? (By Item Number)\n");
	print_tires(tire_array, tire_count);
	int i = 0;
	int delete_item = -1, found = 0;
	scanf("%d", &delete_item);
	for(; i < tire_count - 1; i++)
	{
		if(tire_array[i].item_num == delete_item)
		{	
			found = 1;
			break;
		}
	}
	
	if(found == -1)
	{
		printf("No tire matches item number");
		return;
	}
	
	printf("Are you sure you want to remove this tire? (y or n)");
	int delete = 0;
	scanf("%c", &delete);
	scanf("%c", &delete);
	
	while(delete != 78 && delete != 110 && delete != 89 && delete != 121)
	{
		printf("Invalid Input - Try Again");
	}
	
	if(delete == 89 || delete == 121)
	{
		printf("%d", tire_count);
		for(i = 0; i < tire_count - 1; i++)
		{
			if(tire_array[i].item_num == delete_item)
			{
				for(; i < tire_count - 1; i++)
				{
					tire_array[i] = tire_array[i + 1];
				}
				break;
			}
		}
		tire_count--;
		printf("%d", tire_count);
	}
	return tire_count;
 }
 
Size convert(int height, float width, int rim)
{
	Size x;
	float metric_width, aspect_ratio;
	x.width = round_width(width * 25.4 + .5);	
	x.aspect_ratio = round_aspect_ratio(((height - rim) * 1275) / (width * 25.4) + .5);
	x.rim = rim;
	return x;
}

int round_width(float x)
{
	int y = x;
	if (y%5 != 0 || y%2 == 0)
	{
		y = y / 10.0;
		return (y *10 + 5);
	}
	else
		return y;
}

int round_aspect_ratio(float x)
{
	int y = x;
	if (y%10 > 4)
	{
		if(y%5 > 2)
		{
			y = y / 10.0 +.5;
			return (y *10);
		}
		else
		{
			y = y / 10.0;
			return (y * 10 + 5);
		}
	}
	else
	{
		if (y%5 > 2)
		{
			y = y / 10.0;
			return (y * 10 +5);
		}
		else
		{
			y = y / 10.0;
			return (y * 10);
		}
	}
}
