#ifndef ARRAYSTATS_H
#define ARRAYSTATS_H

#include <iostream>
#include <algorithm> // std::copy
#include <math.h> //sqrtl

namespace ArrayStats{
	struct IncreasingDecreasing{
		long int increasing = 0;
		long int strictlyIncreasing = 0;
		long int decreasing = 0;
		long int strictlyDecreasing = 0;
		IncreasingDecreasing(){}
		IncreasingDecreasing(long int increasing, long int strictlyIncreasing, long int decreasing, long int strictlyDecreasing): increasing(increasing), strictlyIncreasing(strictlyIncreasing), decreasing(decreasing), strictlyDecreasing(strictlyDecreasing){}
		void print();
	};
	
	enum struct Flags : short{
		MEAN = 1, //unordered
		STDEVS = 1 << 1, //unordered
		STDEVP = 1 << 2, //unordered
		MEDIAN = 1 << 3, //ordered	you can do this in O(n) time using quickselect, but it's easy when sorted
		MODE = 1 << 4, //ordered	you can do this with a map, but it's easier when it's sorted
		RANGE = 1 << 5, //unordered
		MAX = 1 << 6, //unordered
		MIN = 1 << 7, //unordered
		INCREASING = 1 << 8, //unordered
		STRICTLY_INCREASING = 1 << 9, //unordered
		DECREASING = 1 << 10, //unordered
		STRICTLY_DECREASING = 1 << 11, //unordered
		SORTED_ALREADY = 1 << 12 //unordered
	};
	const short ALL_STATS = ((short)Flags::SORTED_ALREADY) - ((short)1);
	
	template<typename T>
	void IncreasingDecreasingHandler(IncreasingDecreasing& tracker, IncreasingDecreasing& counter, const T& previous, const T& current, const short& flags = ALL_STATS){
		//This kills a counter streak, but before setting it to zero, it updates the tracker
		#define KILL_COUNTER(flag, name) if((((short)flag) & ((short)flag) )){ \
			if(counter.name > tracker.name ){ \
				tracker.name = counter.name ; \
			} \
			counter.name = 1; \
		}
		
		if(current > previous){ //STRICTLY_INCREASING
			++(counter.increasing);
			++(counter.strictlyIncreasing);
			
			KILL_COUNTER(Flags::DECREASING, decreasing)
			KILL_COUNTER(Flags::STRICTLY_DECREASING, strictlyDecreasing)
		}else if(current == previous){ //INCREASING or DECREASING
			++(counter.increasing);
			++(counter.decreasing);
			
			KILL_COUNTER(Flags::STRICTLY_INCREASING, strictlyIncreasing)
			KILL_COUNTER(Flags::STRICTLY_DECREASING, strictlyDecreasing)
		}else{ //STRICTLY_DECREASING
			++(counter.decreasing);
			++(counter.strictlyDecreasing);
			
			KILL_COUNTER(Flags::STRICTLY_INCREASING, strictlyIncreasing)
			KILL_COUNTER(Flags::INCREASING, increasing)
		}
		#undef KILL_COUNTER
	}
	
	template<typename T>
	struct Stats{
		long double mean = 0.0;
		long double stdevs = 0.0;
		long double stdevp = 0.0;
		T median = -1;
		T mode = -1;
		size_t modeCount = 0;
		T range = -1;
		T max = -1;
		T min = -1;
		IncreasingDecreasing increasingDecreasing;
		
		void print(const short& flags = ALL_STATS){
			if(flags & ((short)Flags::MEAN)) std::cout << "Mean (Average): " << mean << std::endl;
			if(flags & ((short)Flags::STDEVP)) std::cout << "Standard Deviation (Population): " << stdevp << std::endl;
			if(flags & ((short)Flags::STDEVS)) std::cout << "Standard Deviation (Sample): " << stdevs << std::endl;
			if(flags & ((short)Flags::MEDIAN)) std::cout << "Median: " << median << std::endl;
			if(flags & ((short)Flags::MODE)) std::cout << "Mode: " << mode << std::endl;
			if(flags & ((short)Flags::RANGE)) std::cout << "Range: " << range << std::endl;
			if(flags & ((short)Flags::MIN)) std::cout << "Min: " << min << std::endl;
			if(flags & ((short)Flags::MAX)) std::cout << "Max: " << max << std::endl;
			increasingDecreasing.print();
		}
	};
	
	template<typename RandomAccessIterator>
	auto getStats(const RandomAccessIterator begin, const RandomAccessIterator end, const short& flags = ALL_STATS) -> Stats<typename std::remove_const<typename std::remove_reference<decltype(*begin)>::type>::type>{ //End is the pointer sizeof(T) after the last element
		using T = typename std::remove_const<typename std::remove_reference<decltype(*begin)>::type>::type;
		Stats<T> stats;
		if(begin == end) return stats;
		//Then there has to be at least one element
		stats.modeCount = 1;
		stats.increasingDecreasing.increasing = 1;
		stats.increasingDecreasing.decreasing = 1;
		stats.increasingDecreasing.strictlyIncreasing = 1;
		stats.increasingDecreasing.strictlyDecreasing = 1;
		
		const T* sorted = nullptr;
		const bool sortedFlags = (bool)(flags & (((short)Flags::MEDIAN) | ((short)Flags::MODE)));
		const bool sortedAlready = (bool)(flags & ((short)Flags::SORTED_ALREADY));
		const size_t count = std::distance(begin, end);
		if(sortedFlags && (!sortedAlready)){
			T* temp = new T[count]; //Need this temp because of const things (I think)
			std::copy(begin, end, temp); //So not really a single pass anymore
			std::sort(temp, temp+count);
			sorted = temp;
		}else{
			sorted = &(*begin); //We just call it sorted but it actually isn't. Shhhh, they don't need to know
		}
		
		if(flags & ((short)Flags::MEDIAN)){
			if ((count % 2) == 0){
				stats.median = (long double)(sorted[(count / 2) - 1] + sorted[count / 2]) / 2.0L;
			}else {
				stats.median = sorted[count / 2];
			}
		}

		
		if(flags & (((short)Flags::MAX) | ((short)Flags::RANGE))) stats.max = (*begin);
		if(flags & (((short)Flags::MIN) | ((short)Flags::RANGE))) stats.min = (*begin);
		if(flags & ((short)Flags::MODE)) stats.mode = (*begin);
		const T* modeCurrent = &(*begin);
		size_t modeCounter = 1;
		IncreasingDecreasing counter(1, 1, 1, 1);
		for(size_t i = 0; i < count; ++i){
			const T& value = sorted[i];
			
			if(flags & (((short)Flags::MEAN) | ((short)Flags::STDEVS) | ((short)Flags::STDEVP))){
				stats.mean += (long double)value;
			}
			
			if(flags & (((short)Flags::MODE))){
				if((*modeCurrent) == value){
					++modeCounter;
				}else{
					if(modeCounter > stats.modeCount){
						stats.mode = (*modeCurrent);
						stats.modeCount = modeCounter;
					}
					modeCurrent = (&value);
					modeCounter = 0;
				}
			}
			
			if(flags & (((short)Flags::MAX) | ((short)Flags::RANGE)) && (value > stats.max)){
				stats.max = value;
			}else if(flags & (((short)Flags::MIN) | ((short)Flags::RANGE)) && (value < stats.min)){ //Can't set the max and min at the same time
				stats.min = value;
			}
			
			const bool increasingDecreasing = (bool)(flags & (((short)Flags::INCREASING) | ((short)Flags::STRICTLY_INCREASING) | ((short)Flags::DECREASING) | ((short)Flags::STRICTLY_DECREASING)));
			if(increasingDecreasing && i > 0){
				//Increasing and decreasing can't use the sorted array, but we can fix that
				const T& previousValue = begin[i-1];
				const T& currentValue = begin[i];
				
				IncreasingDecreasingHandler(stats.increasingDecreasing, counter, previousValue, currentValue);
			}
		}
		
		if(flags & (((short)Flags::MEAN) | ((short)Flags::STDEVS) | ((short)Flags::STDEVP))){
			stats.mean /= (long double)count;

			if(flags & (((short)Flags::STDEVS) | ((short)Flags::STDEVP))){
				//Have to do another loop because we need the distance from the mean for each element

				long double totalDistanceFromMeanSquared = 0.0L;
				for(size_t i = 0; i < count; ++i){ //Need to loop over it again :(
					const T& x = sorted[i];
					totalDistanceFromMeanSquared += ((long double)x - stats.mean) * ((long double)x - stats.mean); //You don't need abs value because squaring it is always positive
				}

				if(flags & ((short)Flags::STDEVS)){
					stats.stdevs = sqrtl(totalDistanceFromMeanSquared / (count - 1));
				}

				if(flags & ((short)Flags::STDEVP)){
					stats.stdevp = sqrtl(totalDistanceFromMeanSquared / (count));
				}
			}
		}
		
		//make stats max(stats, counter)
		if(modeCounter > stats.modeCount){
			stats.mode = (*modeCurrent);
			stats.modeCount = modeCounter;
		}
		if(counter.increasing > stats.increasingDecreasing.increasing){
			stats.increasingDecreasing.increasing = counter.increasing;
		}
		if(counter.strictlyIncreasing > stats.increasingDecreasing.strictlyIncreasing){
			stats.increasingDecreasing.strictlyIncreasing = counter.strictlyIncreasing;
		}
		if(counter.decreasing > stats.increasingDecreasing.decreasing){
			stats.increasingDecreasing.decreasing = counter.decreasing;
		}
		if(counter.strictlyDecreasing > stats.increasingDecreasing.strictlyDecreasing){
			stats.increasingDecreasing.strictlyDecreasing = counter.strictlyDecreasing;
		}

		stats.range = stats.max - stats.min;
		
		if(sortedFlags && (!sortedAlready)){
			delete[] sorted;
		}
		
		return stats;
	}
}

#endif
