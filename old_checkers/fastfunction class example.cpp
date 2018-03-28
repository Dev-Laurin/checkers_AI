
for (unsigned int i = 1; i<layer_.size();++i) {
	for (unsigned int j = 0; j <layer_[i].size();++j) {
		double temp=0.0;
		for (unsigned int k=0;k<layer_[i-1].size();++k) {
		temp+=layer_[i-1][k].value *(layer_[i][j].weights[k]);
		}
		layer_[i][j].value=sigmoid(temp);
	}
}


double sigmoid(double x) {
	static const double a = 1.0;
	static const double b = 1.0;
	static const double c = 0.0;
	return ((a/(1+exp((-x)/b)))+c);
}
