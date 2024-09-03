#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

bool stop{ false };
std::atomic<int> count(0);

void client()
{
	int count_max = 20;
	while (count.load(std::memory_order_relaxed) < count_max)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		count.fetch_add(1, std::memory_order_relaxed);
		std::cout << "Client count= " << count.load(std::memory_order_relaxed) << "\n";
	}
	stop = true;
}

void operation()
{
	while (count.load(std::memory_order_acquire) != 0 || !stop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		count.fetch_sub(1,std::memory_order_release);
		std::cout << "operation count= " << count.load(std::memory_order_acquire) << "\n";
	}
}

int main()
{
	std::thread t1{ client };
	std::thread t2{ operation};
	t1.join();
	t2.join();

	return EXIT_SUCCESS;
}