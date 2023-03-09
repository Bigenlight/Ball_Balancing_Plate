#define dx -180
#define dy -180
#define x_scale_factor 0.165
#define y_scale_factor 0.164

class mat{
  public:
  double x;
  double y;
  double A[3][1] = {{0},{0},{1}};
  double D[3][1];
  double E[3][1];
  double C[3][3] = {{ x_scale_factor , 0 , 0 } , { 0, y_scale_factor ,0 },{ 0 , 0 , 1 }};
  double B[3][3] = {{ 1 , 0 , dx },{ 0 , 1 , dy },{ 0 , 0 , 1 }};
  int i , j;

  mat(double, double);
  void matcal();
  void update();
};