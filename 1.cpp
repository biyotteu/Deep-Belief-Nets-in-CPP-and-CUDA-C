#include <bits/stdc++.h>

using namespace std;

double dotprod(
    int n,
    double *vec1,
    double *vec2)
{
	int k = n / 4, m = m % 4; // 벡터를 4개의 그룹으로 나눔, 남은 벡터 수 저장 (병렬화를 위한 작업)
	double sum = 0; // 합산 결과
	while (k--) { // 루프 펼치기 방법 (최적화)
		sum += *vec1 * *vec2;
		sum += *(vec1 + 1) * *(vec2 + 1);
		sum += *(vec1 + 2) * *(vec2 + 2);
		sum += *(vec1 + 3) * *(vec2 + 3);
		vec1 += 4;
		vec2 += 4;
	}
	while (m--) sum += *vec1++ * *vec2++;
	return sum;
}

void activity(
    double *input, // 입력 벡터
    double *coefs, // 가중치 벡터
    double *output, // 출력 벡터 (활성 값)
    int ninputs, // 벡터의 길이
    int outlin) { // 선형 판단
	double sum; // 합
	sum = dotprod(ninputs, input, coefs); //입력과 가중치 벡터 내적
	sum += coefs[ninputs]; // 바이어스 값 추가
	if (outlin) *output = sum; //선형
	else *output = 1.0 / (1.0 + exp(-sum)); // 로지스틱
}


static void trial_thr(
    double *input, // 입력 벡터
    int n_all, // 입력 층을 제외한 모든 레이어 수(히든 + 아웃)
    int n_model_inputs, // 입력 벡터 길이
    double *outputs, // ntarg만큼의 길이를 갖는 모델의 출력 벡터
    int ntarg, // 모델의 최종 출력 수
    int *nhid_all, // nhid_all[i]은 i번째 은닉 레이어에 존재하는 뉴런 수
    double *weights_opt[], // 은닉 레이어의 가중치 벡터
    double *hid_act[], // 은닉 레이어의 활성화 벡터 (입력 레이어를 제외)
    double *final_layer_weights, //
    int classifier // 0이 아니면 SoftMax로 출력, 1이면 선형 조합으로 출력
)
{
	int i, ilayer;
	double sum;
	for (ilayer = 0; ilayer < n_all) {
		if (ilayer == 0 && n_all == 1) { // 입력이 곧바로 출력 (은닉 레이어 없음)
			for (i = 0; i < ntarg; i++) 
				activity(input, final_layer_weights + i * (n_model_inputs + 1), outputs + i, n_model_inputs, 1);
			// 입력 레이어의 뉴련 수(n_model_inputs) + 1(바이어스 값) 개의 가중치, 다음 층 -> 출력 레이어, 선형
		}
		else if (ilayer == 0) {
			for (i = 0; i < nhid_all[ilayer]; i++) 
				activity(input, weights_opt[ilayer] + i * (n_model_inputs + 1), hid_act[ilayer] + i, n_model_inputs, 0);
		}
		else if (ilayer < n_all - 1) { // 출력 레이어가 아닌 중간 은닉 레이어인가?
			for (i = 0; i < nhid_all[ilayer]; i++) 
				activity(hid_act[ilayer - 1], weights_opt[ilayer] + i * (nhid_all[ilayer - 1] + 1), hid_act[ilayer] + i, nhid_all[ilayer - 1], 0);
		}
		else{ // 출력 레이어
			for(i=0;i<ntarg;i++)
				activity(hid_act[ilayer-1], final_layer_weights + i*(nhid_all[ilayer-1]+1),outputs+i,nhid_all[ilayer-1],1);
		}
	}
	if(classifier){
		sum = 0.0;
		for (i=0;i<ntarg;i++){
			if(outputs[i] < 300.0) outputs[i] = exp(outputs[i]);
			else outputs[i] = exp(300.0);
			sum += outputs[i];
		}
		for(i=0;i<ntarg;i++) outputs[i] /= sum;
	}
}
double batch_gradient(
	int istart, // 입력 행렬의 첫 번째 데이터 인덱스
	int istop, // 지난 마지막 데이터의 인덱스
	double *input, // 입력 행렬 데이터의 길이 = max_neurons
	double *targets, // 목표 행렬 데이터의 길이 = ntarg
	int n_all, // 출력 포함 입력 제외한 레이어 수
	int n_all_weights, // 마지막 레이어와 모든 바이어스 항을 포함한 가중치 수
	int n_model_inputs, // 모델 입력의 개수
	double *outputs, //모델의 출력 벡터
	int ntarg, // 출력의 개수
	int *nhid_all, // nhid_all[i]은 i번째 은닉 레이어에 존재하는 뉴런의 개수
	double *weights_opt[], // weights_opt[i]는 i번째 은닉 레이어의 가중치 벡터를 가르키는 포인터
	double *hid_act[], // i번째 은닉 레이어의 활성화 벡터
	int max_neurons, // 입력 행렬의 열의 개수
	double *this_delta, // 현재 레이어에 대한 델타 변수를 가르키는 포인터
	double *prior_delta, // 다음 단계에 사용하기 위해 이전 레이어에서 미리 저장해놓은 델타 변수
	double **grad_ptr, // grad_ptr[i]는 i번째 레이어의 기울기를 가리키는 포인터
	double *final_layer_weights, // 마지막 레이어의 가중치
	double *grad, // 계산된 모든 기울기
	int classifier // 0이 아니면 SoftMax, 0이면 선형
	)
{
	
}
int main() {

}