#include "tgaimage.h"
#include"model.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,255,0,255);
Model *model = NULL;
const int width  = 800;
const int height = 800;
int Lmin[height+1],Rmax[height+1];
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
			Rmax[y] = std::max(Rmax[y],x);
			Lmin[y] = std::min(Lmin[y],x);
		}
		else{
			image.set(y,x,color);
			Rmax[x] = std::max(Rmax[x],y);
			Lmin[x] = std::min(Lmin[x],y);
		}
		error2 += derror2;
		if(error2>dx){
			y+= (y0>y1 ? -1:1);
			error2-=dx*2;
			
		}
	}
}
void triangle(Vec2i t0,Vec2i t1,Vec2i t2,TGAImage& image,const TGAColor& color){
	Vec2i t[3] = {t0,t1,t2};
	for(int i=2;i>=0;--i){
		for(int j=0;j<i;++j){
			if(t[j].y>t[j+1].y){
				swap(t[j],t[j+1]);
			}
		}
	}
	for(int h=t[0].y;h<=t[2].y;++h){
		Rmax[h] = 0;
		Lmin[h] = width;
	}
	line(t[0],t[1],image,color);
	line(t[0],t[2],image,color);
	line(t[1],t[2],image,color);
	for(int h=t[0].y;h<=t[2].y;++h){
		for(int i = Lmin[h];i<=Rmax[h];++i){
			image.set(i,h,color);
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
