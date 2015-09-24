#include "DETECTOR.h"

using namespace cv;
using namespace std;

detector::detector(){
	this->hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	this->scram = PRESCRAM;
	this->color = BGR;
}

detector::~detector(){
	destroyWindow("Pre-detection");
	destroyWindow("Post-detection");
}

void * detector::process(IplImage *frame){

	switch (this->scram)
	{
	case PRESCRAM:
		//Predetection
		this->framepre = predetection(Mat(frame,1));
		break;

	case POSTSCRAM:
	{
		//Predetection + Scrambling
		this->frameidct = scrambling(Mat(frame,1), this->color);
		//Detection + Scrambling + Postdetection
		this->framepost = postdetection(this->frameidct);
		break;
	}
	default:
		printf("Parametro especificado incorrectamente.\n");
		break;
	}

	return NULL;
}

void detector::showresults(){

	/*switch (this->scram)
	{
		case PRESCRAM:
		{
			namedWindow("Pre-detection", WINDOW_NORMAL);
			imshow("Pre-detection", this->framepre);
			break;
		}
		case POSTSCRAM:
		{
			namedWindow("Post-detection", WINDOW_NORMAL);
			imshow("Post-detection", this->framepost);
			break;
		}
	}
	waitKey(1);*/
}

int detector::getParam(int parametro, int *valor_int){
	switch (parametro)
	{
	case COLOR:
		*valor_int = this->color;
		break;
	case SCRAM:
		*valor_int = this->scram;
		break;
	default:
		printf("Parámetro especificado incorrectamente.\n");
		break;
	}
	return 1;
}

int detector::setParam(int parametro, int valor_int){
	switch (parametro)
	{
	case COLOR:
		this->color = valor_int;
		break;
	case SCRAM:
		this->scram = valor_int;
		break;
	default:
		printf("Parámetro especificado incorrectamente.\n");
		break;
	}
	return 1;
}

Mat detector::predetection(Mat &frame){
	
	Mat frametemp;

	frame.copyTo(frametemp);

	static vector<float> detector = HOGDescriptor::getDefaultPeopleDetector();
	if (!detector.size()) {
		fprintf(stderr, "ERROR: getDefaultPeopleDetector returned NULL\n");     
	}
	this->hog.setSVMDetector(detector);

	fflush(stdout);

	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	this->hog.detectMultiScale(frametemp, this->found, 0, Size(8, 8), Size(32, 32), 1.05, 2);

	size_t i, j;
	for (i = 0; i < this->found.size(); i++)
	{
		this->r1 = this->found[i];
		for (j = 0; j < this->found.size(); j++)
			if (j != i && (this->r1 & this->found[j]) == this->r1)
				break;
		if (j == this->found.size()){
			this->found_filtered.push_back(this->r1);	
		}
	}

	for (i = 0; i < this->found_filtered.size(); i++)
	{
		this->r2 = this->found_filtered[i];
		int x1, y1, x2, y2;
		x1 = this->r2.x;
		y1 = this->r2.y;
		x2 = this->r2.x + this->r2.width;
		y2 = this->r2.y + this->r2.height;

		// the HOG detector returns slightly larger rectangles than the real objects.
		// so we slightly shrink the rectangles to get a nicer output.
		this->r2.x += cvRound(this->r2.width*0.1);
		this->r2.width = cvRound(this->r2.width*0.8);
		this->r2.y += cvRound(this->r2.height*0.07);
		this->r2.height = cvRound(this->r2.height*0.8);
		rectangle(frametemp, this->r2.tl(), this->r2.br(), cv::Scalar(255, 0, 0), 1);
	}
	
	found_filtered.clear();

	return frametemp;
}

Mat detector::imagescram(Mat &blockdct){

	// Matriz con la que trabajamos
	Mat blocktemp = blockdct;
	Mat random(8, 8, CV_64F, Scalar(0));

	// Creamos la matriz de ceros y unos aleatorios
	srand(5);
	randu(random, 0, 1);

	for (int x = 0; x<8; x++) {
		for (int y = 0; y<8; y++) {
			if (random.at<double>(x, y) < 0.5) {
				random.at<double>(x, y) = 0;
			}
			else {
				random.at<double>(x, y) = 1;
			}
		}
	}

	// Recorremos el bloque DCT y
	// en caso de 1 se cambia el signo
	for (int x = 0; x<8; x++) {
		for (int y = 0; y<8; y++) {
			if (x == 0 && y == 0){
				blocktemp.at<double>(x, y) = blocktemp.at<double>(x, y);
			}
			else if (random.at<double>(x, y) == 1) {

				//double a = imagedct.at<double>(x,y);
				//a = -a;
				blocktemp.at<double>(x, y) = -blocktemp.at<double>(x, y);


			}
		}
	}
	blockdct = blocktemp;
	return blockdct;
}

Mat detector::idctcall(Mat &roidct) {

	Mat roitemp(8, 8, CV_64F, Scalar(0));
	roitemp = roidct;
	Mat block(8, 8, CV_64F, Scalar(0));
	Mat blockidct(8, 8, CV_64F, Scalar(0));
	Mat roiidct(roitemp.rows, roitemp.cols, CV_64F);


	for (int y = 0; y<roitemp.rows; y += 8) {
		for (int x = 0; x<roitemp.cols; x += 8) {

			// Obtenemos el bloque 8x8
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {

					block.at<double>(xx, yy) = roitemp.at<double>(xx + y, yy + x);
					//cout << "block = "<< endl << " "  << block << endl << endl;
				}
			}

			// Aplicamos la IDCT al bloque
			idct(block, blockidct);
			//cout << "blockdct = "<< endl << " "  << blockdct << endl << endl;

			// Reconstruimos la imagen DCT
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {
					roiidct.at<double>(xx + y, yy + x) = blockidct.at<double>(xx, yy);
					//cout << "imagendct = "<< endl << " "  << imagedct << endl << endl;
				}
			}
		}
	}

	// Convertimos los float a 8 bits por pixel para luego mostrar el resultado
	roiidct.convertTo(roitemp, CV_8UC1);

	return roitemp;
}

Mat detector::dctcall2(Mat &image, Rect &roi) {

	Mat imagetemp = image;

	// Make sure the both images dimensions are a multiple of 8
	int w = imagetemp.cols;
	int h = imagetemp.rows;
	int w2, h2;

	if (w % 8 == 0)
		w2 = w;
	else {
		w2 = w;
		while (w2 % 8 != 0) {
			w2 = w2 + 1;
		}
	}
	if (h % 8 == 0)
		h2 = h;
	else{
		h2 = h;
		while (h2 % 8 != 0) {
			h2 = h2 + 1;
		}
	}
	copyMakeBorder(imagetemp, imagetemp, 0, h2 - h, 0, w2 - w, IPL_BORDER_REPLICATE);

	//rectangle(image, rtemp.tl(), rtemp.br(), cv::Scalar(0,255,0), 3);
	//line(image, ini, fin, Scalar(250,0,0));

	//La función dct() utiliza valores float, conversión
	Mat roitemp = Mat(imagetemp.rows, imagetemp.cols, CV_64F);
	imagetemp.convertTo(roitemp, CV_64F);


	//Divide la ROI en bloques 8x8 y aplica el scrambling
	Mat block(8, 8, CV_64F, Scalar(0));
	Mat blockdct(8, 8, CV_64F, Scalar(0));
	Mat roidct(roitemp.rows, roitemp.cols, CV_64FC3, Scalar(0));


	for (int y = 0; y<roitemp.rows; y += 8) {
		for (int x = 0; x<roitemp.cols; x += 8) {

			// Obtenemos el bloque 8x8
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {

					block.at<double>(xx, yy) = roitemp.at<double>(xx + y, yy + x);
					//cout << "block = "<< endl << " "  << block << endl << endl;
				}
			}

			// Aplicamos la DCT al bloque
			dct(block, blockdct);

			// Aplicamos el scrambling al bloque
			//Mat blockscram(8,8,CV_64F,Scalar(0));

			//blockscram = imagescram(blockdct);
			//blockdct = blockscram;

			// Reconstruimos la imagen DCT
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {
					roidct.at<double>(xx + y, yy + x) = blockdct.at<double>(xx, yy);
				}
			}

		}
	}
	return roidct;
}

Mat detector::dctcall(Mat &image, Rect &roi) {

	Mat imagetemp = image;

	// Make sure the both images dimensions are a multiple of 8
	int w = imagetemp.cols;
	int h = imagetemp.rows;
	int w2, h2;

	if (w % 8 == 0)
		w2 = w;
	else {
		w2 = w;
		while (w2 % 8 != 0) {
			w2 = w2 + 1;
		}
	}
	if (h % 8 == 0)
		h2 = h;
	else{
		h2 = h;
		while (h2 % 8 != 0) {
			h2 = h2 + 1;
		}
	}
	copyMakeBorder(imagetemp, imagetemp, 0, h2 - h, 0, w2 - w, IPL_BORDER_REPLICATE);

	//rectangle(image, rtemp.tl(), rtemp.br(), cv::Scalar(0,255,0), 3);
	//line(image, ini, fin, Scalar(250,0,0));

	//La función dct() utiliza valores float, conversión
	Mat roitemp = Mat(imagetemp.rows, imagetemp.cols, CV_64F);
	imagetemp.convertTo(roitemp, CV_64F);


	//Divide la ROI en bloques 8x8 y aplica el scrambling
	Mat block(8, 8, CV_64F, Scalar(0));
	Mat blockdct(8, 8, CV_64F, Scalar(0));
	Mat roidct(roitemp.rows, roitemp.cols, CV_64FC3, Scalar(0));


	for (int y = 0; y<roitemp.rows; y += 8) {
		for (int x = 0; x<roitemp.cols; x += 8) {

			// Obtenemos el bloque 8x8
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {

					block.at<double>(xx, yy) = roitemp.at<double>(xx + y, yy + x);
					//cout << "block = "<< endl << " "  << block << endl << endl;
				}
			}

			// Aplicamos la DCT al bloque
			dct(block, blockdct);

			// Aplicamos el scrambling al bloque
			Mat blockscram(8, 8, CV_64F, Scalar(0));

			blockscram = imagescram(blockdct);
			blockdct = blockscram;

			// Reconstruimos la imagen DCT
			for (int xx = 0; xx<8; xx++){
				for (int yy = 0; yy<8; yy++) {
					roidct.at<double>(xx + y, yy + x) = blockdct.at<double>(xx, yy);
				}
			}

		}
	}
	return roidct;
}

Mat detector::scrambling(Mat &framedetec, int parametro){

	Mat rois;
	Mat frametemp, frameidct, Bframedct, Bframeidct, Gframedct, Gframeidct, Rframedct, Rframeidct;
	framedetec.copyTo(frametemp);
	vector<Rect> found_filtered;

	static vector<float> detector = HOGDescriptor::getDefaultPeopleDetector();
	if (!detector.size()) {
		fprintf(stderr, "ERROR: getDefaultPeopleDetector returned NULL\n");     
	}
	this->hog.setSVMDetector(detector);

	fflush(stdout);

	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	this->hog.detectMultiScale(frametemp, this->found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).


	size_t i, j, k;
	for (i = 0; i < this->found.size(); i++)
	{
		this->r1 = this->found[i];
		for (j = 0; j < this->found.size(); j++)
			if (j != i && (this->r1 & this->found[j]) == this->r1)
				break;
		if (j == this->found.size()){
			found_filtered.push_back(this->r1);	
		}
	}

	// Si hay alguna ROI hacemos la DCT y scrambling
	if (found_filtered.size() != 0) {
		for (k = 0; k < found_filtered.size(); k++)
		{

			Mat imageroi;

			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			this->r1.x += cvRound(this->r1.width*0.05);
			this->r1.x = this->r1.x + this->r1.width / 4;
			this->r1.width = cvRound(this->r1.width*0.8);
			this->r1.width = this->r1.width / 2;
			this->r1.y += cvRound(this->r1.height*0.05);
			this->r1.height = cvRound(this->r1.height*0.8);
			// Cojemos la cuarta parte para coger sÛlo la cara
			this->r1.height = this->r1.height / 4;


			// Si la ROI se sale del frame... o es negativa
			if (this->r1.x < 0 || this->r1.y <0 || this->r1.x>frametemp.cols - this->r1.width || this->r1.y>frametemp.rows - this->r1.height){
				return frametemp;
				//r.width = 8;
				//imageroi = frametemp(r);
			}
			else {
				imageroi = frametemp(this->r1);
			}

			switch(parametro)
			{
				case BGR:
				{
					//Separaramos en tres canales (BGR) la imagen ROI
					Mat channel[3], roiR, roiG, roiB;
					split(imageroi, channel);
					roiB = channel[0];
					roiG = channel[1];
					roiR = channel[2];

					//Llamamos a DCT y IDCT para cada canal
					Bframedct = dctcall(roiB, this->r1);
					Bframeidct = idctcall(Bframedct);
					Gframedct = dctcall(roiG, this->r1);
					Gframeidct = idctcall(Gframedct);
					Rframedct = dctcall(roiR, this->r1);
					Rframeidct = idctcall(Rframedct);

					//Copiamos el resultado de los scrambling en cada canal por separado
					Bframeidct.copyTo(channel[0]);
					Gframeidct.copyTo(channel[1]);
					Rframeidct.copyTo(channel[2]);

					//Juntamos los tres canales en la matriz que contiene sólo la parte con scrambling
					merge(channel, 3, frameidct);
				break;
				}
				

				case GR:
				{
					//Separaramos en tres canales (BGR) la imagen ROI
					Mat channel[3], roiR, roiG, roiB;
					split(imageroi, channel);
					roiB = channel[0];
					roiG = channel[1];
					roiR = channel[2];

					//Llamamos a DCT y IDCT para cada canal
					Bframedct = dctcall2(roiB, this->r1);
					Bframeidct = idctcall(Bframedct);
					Gframedct = dctcall(roiG, this->r1);
					Gframeidct = idctcall(Gframedct);
					Rframedct = dctcall(roiR, this->r1);
					Rframeidct = idctcall(Rframedct);

					//Copiamos el resultado de los scrambling en cada canal por separado
					Bframeidct.copyTo(channel[0]);
					Gframeidct.copyTo(channel[1]);
					Rframeidct.copyTo(channel[2]);

					//Juntamos los tres canales en la matriz que contiene sólo la parte con scrambling
					merge(channel, 3, frameidct);
				break;
				}

				case YCRCB:
				{
					//Separamos en los canales YCrCb la imagen ROI
					Mat ycbcr, chan[3], y, cb, cr;
					cvtColor(imageroi,ycbcr,CV_BGR2YCrCb); 
					split(ycbcr,chan); 
					y = chan[0];
					cb = chan[1];
					cr = chan[2];

					Mat yframedct = dctcall(y,this->r1);
					Mat yframeidct = idctcall(yframedct);
					Mat cbframedct = dctcall(cb,this->r1);
					Mat cbframeidct = idctcall(cbframedct);
					Mat crframedct = dctcall(cr,this->r1);
					Mat crframeidct = idctcall(crframedct);

					yframeidct.copyTo(chan[0]);
					cbframeidct.copyTo(chan[1]);
					crframeidct.copyTo(chan[2]);

					merge(chan,3,frameidct);

					//cvtColor(frameidct,frameidct,CV_YCrCb2BGR);

				break;
				}
				
				case Y:
				{
					//Separamos en los canales YCrCb la imagen ROI
					Mat ycbcr, chan[3], y, cb, cr;
					cvtColor(imageroi,ycbcr,CV_BGR2YCrCb); 
					split(ycbcr,chan); 
					y = chan[0];
					cb = chan[1];
					cr = chan[2];

					Mat yframedct = dctcall(y,this->r1);
					Mat yframeidct = idctcall(yframedct);
					Mat cbframedct = dctcall2(cb,this->r1);
					Mat cbframeidct = idctcall(cbframedct);
					Mat crframedct = dctcall2(cr,this->r1);
					Mat crframeidct = idctcall(crframedct);

					yframeidct.copyTo(chan[0]);
					cbframeidct.copyTo(chan[1]);
					crframeidct.copyTo(chan[2]);

					merge(chan,3,frameidct);

					//cvtColor(frameidct,frameidct,CV_YCrCb2BGR);
						
				break;
				}

				default:
					printf("Parametro especificado incorrectamente.\n");
					break;
			}

			//Obtenemos los datos de anchura y altura de la roi con scrambling
			int width = frameidct.cols;
			int height = frameidct.rows;
			this->r2.x = this->r1.x;
			this->r2.width = width;
			this->r2.y = this->r1.y;
			this->r2.height = height;

			// Si la ROI se sale del frame... o es negativa
			if (this->r2.x < 0 || this->r2.y <0 || this->r2.x>frametemp.cols - this->r2.width || this->r2.y>frametemp.rows - this->r2.height){
				if (this->r2.y > this->r2.height){
					frameidct.copyTo(frametemp.rowRange(this->r2.y - this->r2.height / 2, this->r2.y + this->r2.height / 2).colRange(this->r2.x - this->r2.width / 2, this->r2.x + this->r2.width / 2));
				}
				else{
					frameidct.copyTo(frametemp.rowRange(this->r2.y, this->r2.y + this->r2.height).colRange(this->r2.x - this->r2.width / 2, this->r2.x + this->r2.width / 2));
				}
			}
			else {
				frameidct.copyTo(frametemp.rowRange(this->r2.y, this->r2.y + this->r2.height).colRange(this->r2.x, this->r2.x + this->r2.width));
			}

			// Copiamos el bloque con scrambling en el frame original
			// Para ello necesitamos indicarle la posición

		}
	}
	return frametemp;
}

Mat detector::postdetection(Mat &framedetec){

	Mat rois;
	Mat frametemp;
	framedetec.copyTo(frametemp);

	static vector<float> detector = HOGDescriptor::getDefaultPeopleDetector();
	if (!detector.size()) {
		fprintf(stderr, "ERROR: getDefaultPeopleDetector returned NULL\n");     
	}
	this->hog.setSVMDetector(detector);

	fflush(stdout);

	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	this->hog.detectMultiScale(frametemp, this->found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).

	size_t i, j;
	for (i = 0; i < this->found.size(); i++)
	{
		this->r1 = this->found[i];
		for (j = 0; j < this->found.size(); j++)
			if (j != i && (this->r1 & this->found[j]) == this->r1)
				break;
		if (j == this->found.size()){
			this->found_filtered.push_back(this->r1);
		}

	}

	//if (found_filtered.size() != 0){
		for (i = 0; i < this->found_filtered.size(); i++)
		{
			this->r2 = this->found_filtered[i];
			int x1, y1, x2, y2;
			x1 = this->r2.x;
			y1 = this->r2.y;
			x2 = this->r2.x + this->r2.width;
			y2 = this->r2.y + this->r2.height;

			// the HOG detector returns slightly larger rectangles than the real objects.
			// so we slightly shrink the rectangles to get a nicer output.
			this->r2.x += cvRound(this->r2.width*0.1);
			this->r2.width = cvRound(this->r2.width*0.8);
			this->r2.y += cvRound(this->r2.height*0.07);
			this->r2.height = cvRound(this->r2.height*0.8);
			rectangle(frametemp, this->r2.tl(), this->r2.br(), cv::Scalar(0, 0, 255), 1);
		}

	found_filtered.clear();

	return frametemp;
}