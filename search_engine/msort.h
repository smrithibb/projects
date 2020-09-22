#include <vector>


template <class T, class Compare >
void msorthelper(std::vector<T>& list, std::vector<T>& outputList, int start, int end, Compare comp );

template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp );

template <class T, class Compare >
void merge(std::vector<T>& list, std::vector<T>& outputList, int start1, int end1, int start2, int end2, Compare comp);



template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp ){
	int start = 0;
	int end = list.size();
	std::vector<T> outputList(end);
	msorthelper(list, outputList,  start,  end, comp );
}

template <class T, class Compare >
void msorthelper(std::vector<T>& list, std::vector<T>& outputList, int start, int end, Compare comp ){
	
	if(start >= end-1){
		return;
	}
	int mid = (start+end)/2;
	//left branch
	msorthelper(list, outputList, start, mid, comp);
	//right branch
	msorthelper(list, outputList, mid, end, comp);
	merge(list, outputList, start, mid, mid, end, comp);
	
	for( int i=start; i < end; i++){
		list[i] = outputList[i];
	}

}

template <class T, class Compare >
void merge(std::vector<T>& list, std::vector<T>& outputList, int s1, int e1, int s2, int e2, Compare comp){
	int outS = s1;
	while(s1 < e1 && s2 < e2){
		if( comp( list[s1], list[s2] )  ){
        	outputList[outS] = list[s1];
			s1++;
			outS++;
      	}
		else{
			outputList[outS] = list[s2];
			s2++;
			outS++;

		}
	}

	if(s1 == e1){
		while(s2 != e2){
			outputList[outS] = list[s2];
			s2++;
			outS++;
			
		}
	}

	else if(s2 == e2){
		while(s1 != e1){
			outputList[outS] = list[s1];
			s1++;
			outS++;
		}

	}


}








