#include "tgaimage.h"
#include"model.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,255,0,255);
Model *model = NULL;
const int width  = 800;
const int height = 800;
void line(Vec2i v0,Vec2i v1,TGAImage& image,const TGAColor& color){
	bool steep = false;
	int x0 = v0.x,y0 = v0.y;
	int x1 = v1.x,y1 = v1.y;
	if(std::abs(x0-x1)<std::abs(y0-y1)){
		std::swap(x0,y0);
		std::swap(x1,y1);
		steep = true;
	}
	if(x0>x1){
		std::swap(x0,x1);
		std::swap(y0,y1);
	}
	int dx = (x1-x0);
	int dy = (y1-y0);
	int derror2 = 2*std::abs(dy);
	int error2 = 0;
	int y = y0;
	for(int x = x0;x<=x1;++x){
		if(!steep){
			image.set(x,y,color);
		}
		else{
			image.set(y,x,color);
		}
		error2 += derror2;
		if(error2>dx){
			y+= (y0>y1 ? -1:1);
			error2-=dx*2;
			
		}
	}
}
void triangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage& image,const TGAColor& color){
	if(t0.y == t1.y&&t1.y==t2.y) return;
	if(t0.y>t1.y) std::swap(t0,t1);
	if(t1.y>t2.y) std::swap(t1,t2);
	if(t0.y>t1.y) std::swap(t0,t1);
	int total_height = t2.y - t0.y;
	for(int i=0;i<total_height;++i){
		bool second_half = (i>t1.y-t0.y) || (t1.y == t0.y);
		int segment_height = second_half ? t2.y-t1.y:t1.y-t0.y;
		float alpha = float(i)/total_height;
		float beta = (float(i) - (second_half? (t1.y-t0.y):0))/segment_height;
		Vec2i A = t0 + (t2-t0)*alpha;
		Vec2i B = second_half?(t1+(t2-t1)*beta):(t0+(t1-t0)*beta);
		if(A.x>B.x) std::swap(A,B);
		for(int j=A.x;j<=B.x;++j)
		{
			image.set(j,t0.y+i,color);
		}
	}

}
int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)}; 
	Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)}; 
	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)}; 
	triangle(t0[0], t0[1], t0[2], image, red); 
	triangle(t1[0], t1[1], t1[2], image, white); 
	triangle(t2[0], t2[1], t2[2], image, green);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
