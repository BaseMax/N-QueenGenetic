// Max Base
// https://github.com/BaseMax/N-QueenGenetic
// بارگزاری کتابخانه ها
#include <iostream>
#include <algorithm> // sort, random_shuffle
#include <sstream> // ostringstream
// #include <string>
#include <map>
#include <queue> // vector, .push_back()

// بارگزاری استاندارد ها به برنامه جاری جهت دسترسی راحت تر به آنها
using namespace std;

// تعریف یک نوع جدید به نام individual مشابه نوع های دیگری مثل اینت
// این نوع شامل دو زیر مقدار است که یکی از نوع رشته و دیگری از نوع عدد است.
typedef struct {
	// راه حل مورد از جنس رشته که در آن به تعداد مورد نظر (در اینجا مثلا ۸) عدد وجود دارد.
	string way;
	// مقدار عددی
	int cost;
} individual;

vector<individual*> population;
int chessBoardSize;
int maxSolutions;
int initialPopulationCount = 10;

int fitValue(string way) {
	// متغییر fitness یک مقدار عددی است.
	int fitness=(chessBoardSize*(chessBoardSize-1))/2;
	// حذف مواردی که جفت در یک ردیف و یک قطر هستند.
	for(int i=0; i<chessBoardSize; i++)
		for(int j=i+1; j<chessBoardSize; j++)
			if((way[i] == way[j]) ||  (i-way[i] == j-way[j]) || (i+way[i] == j+way[j]))
				// در صورتی که شرط ها برقرار بود یک مقدار از متغییر fitness کم می کنیم.
				fitness--;
	// مقدار نهایی fitness را برگشت می دهیم.
	return fitness;
}

individual* reproduce(individual *a, individual *b) {
	// ایجاد یک شی جدید با نوع individual
	individual *item = new individual;
	// ایجاد یک عدد تصادفی که کمتر از chessBoardSize باشد.
	int c = rand()%chessBoardSize;
	// ساخت یک راه حل جدید با ترکیب راه حل های دو عنصر
	item->way =
				// از رشته عنصر x از کاراکتر اولی تا c ام را میگیریم
				(a->way).substr(0, c) +
				 // و ادامه رشته را از عنصر y از کاراکتر c ام تا chessBoardSize-c+1 ام میگیریم
				(b->way).substr(c, chessBoardSize-c+1)
	;
	// ایجاد مقدار عددی طبق مقدار راه حل که یک رشته است.
	item->cost = fitValue(item->way);
	return item;
}

// این تابع را می نویسم تا یک معیار برای مرتب کردن لیست تعیین کنیم.
// می تواند باعث ایجاد لیست صعوی یا نزولی شود.
// می توانید علامت بزرگتر را به کوچکتر تغییر دهید تا عملکرد برعکس باشد و لیست مرتب شده برعکس شود.
bool compare(individual *a, individual*b) {
	return(a->cost > b->cost);
}

individual* GA() {
	int random1, random2;
	individual *individual1, *individual2, *item;
	bool found=0;
	// تا زمانی که پیدا شود.
	while(!found) {
		// ایجاد یک متغییر کمکی برای متغییر population که متغییر اصلی است.
		vector<individual*> populationNew;
		// به تعداد اعضای لیست population کد تکرار و اندیس آی افزایش یابد.
		for(unsigned int i=0; i<population.size(); i++) {
			// مرتب کردن population با استفاده از الگوریتم آماده سی پلاس پلاس
			// این مرتب سازی بر پایه تابع compare انجام می شود.
			sort(population.begin(), population.end(), compare);

			// ایجاد یک عدد تصادفی که همیشه کمتر از ۲ باشد.
			// می تواند ۱ یا ۰ باشد.
			random1 = rand()%population.size() %2;
			random2 = rand()%population.size() %2;

			// انتخاب دو مورد به تصادف از لیست population
			individual1 = population[random1];
			individual2 = population[random2];

			item = reproduce(individual1, individual2);

			if(rand()%2==0)
				// این کد ممکن است اجرا شود و ممکن است اجرا نشود و ما این تصادفی بودن را می خواهیم ایجاد کنیم.
				item->way[rand()%(chessBoardSize)+1] = (rand()%(chessBoardSize)+1)+48;

			if(fitValue(item->way)==((chessBoardSize*(chessBoardSize-1))/2)) {
				// در اینجا اعلام می کنیم که یک دادع پیدا شد.
				found=1;
				// نتیجه را برگشت می دهیم.
				return item;
			}
			// درج این مورد به لیست
			populationNew.push_back(item);
		}
		//
		population = populationNew;
	}
	return item;
}

int main() {
	// این کد برای ایجاد عدد تصادفی ایمن و مناسب تر نوشته می شود.
	srand(time(0));
	// تنظیم اندازه صفحه شطرنج
	chessBoardSize=8;
	// تنظیم نهایت تعداد ممکن برای حالت های چینش وزیر ها
	// این یک محدودیت برای برنامه است.
	// این مسعله برای ورودی ۸ جواب ۹۲ را باید تولید کند.
	// می توانید در این مثال 
	maxSolutions=92;
	// ایجاد یک لیست برای موارد پیدا شده
	map<string, int> solutions;
	// تعداد پیدا شده ها را در ابتدا صفر قرار میدهیم.
	int numFound=0;
	// تا زمانی که تعداد پیدا شده ها برابر با محدودیت ایجاد شده نباشد.
	while(numFound!=maxSolutions) {
		// تعریف متغییر با مقدار یک رشته خالی
		string tempWay="";
		// از اندیس ۱ تا خود عدد chessBoardSize تکرار شویم و اندیس افزایش یابد.
		for(int i=1; i<=chessBoardSize; i++) {
			// ایجاد یک متغییر کمکی به نام ostr
			// همچنین با استفاده از این می توانیم به سادگی عدد را به رشته تبدیل نمایید.
			ostringstream ostr;
			ostr<<i;
			// افزودن عدد و رشته به متغییر اصلی tempWay همراه با نگه داشتن مقدار های قبلی متغییر tempWay
			tempWay += ostr.str();
		}

		// ایجاد یک متغییر کمکی از جنس individual
		individual *temp;
		// به تعداد initialPopulationCount حلقه تکرار شود و اندیس از ۰ تا قبل از initialPopulationCount افزایش یابد.
		for(int i=0; i<initialPopulationCount; i++) {
			// باعث بهم زدن ترتیب لیست می شود
			// این اتفاق بصورت تصادفی انجام می شود.
			// هر دفعه ممکن است ترتیب اعضا را جابجا کنند و باعث ایجاد یک لیست جدید و دیگری شود.
			// در نظر داشته باشید که tempWay یک رشته است.
			random_shuffle(tempWay.begin(), tempWay.end());
			// تعریف یک متغییر کمکی
			temp   = new individual;
			temp->way = tempWay;
			temp->cost = fitValue(tempWay);
			// درج متغییر کمکی به لیست population
			population.push_back(temp);
		}

		// پیدا کردن راه حل
		individual *solution = GA();
		// توجه شود که راه حل یک رشته هست.
		// بررسی شود که راه حل از قبل پیدا شده است یا یک راه حل جدید است.
		if(!solutions[solution->way]) {
			// هنگامی که شرط اجرا شود و داخل آن بیاییم به این معنی است که راه حل یک راه حل جدید بوده است.
			// و قبلا این راه حل را پیدا نکرده بودیم.
			solutions[solution->way]=1;
			// نمایش راه حل بهمراه شماره اندیس
			cout<<"Possible Solution #"<<(++numFound)<<":\t"<<solution->way<<endl;
		}
	}
	return 0;
}
