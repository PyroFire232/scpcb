#include <set>
#include <vector>

#include "cachedtexture.h"

#include "../gxruntime/gxutil.h"
#include "../gxruntime/gxruntime.h"
#include "../gxruntime/gxscene.h"

int active_texs;

extern gxRuntime *gx_runtime;
extern gxGraphics *gx_graphics;

std::set<CachedTexture::Rep*> CachedTexture::rep_set;

static String path;

struct CachedTexture::Rep{
	int ref_cnt;
	String file;
	int flags,w,h,first;
	std::vector<gxCanvas*> frames;

	Rep( int w,int h,int flags,int cnt ):
	ref_cnt(1),flags(flags),w(w),h(h),first(0){
		++active_texs;
		while( cnt-->0 ){
			if( gxCanvas *t=gx_graphics->createCanvas( w,h,flags ) ){
				frames.push_back( t );
			}else break;
		}
	}

	Rep( String f,int flags,int w,int h,int first,int cnt ):
	ref_cnt(1),file(f),flags(flags),w(w),h(h),first(first){
		++active_texs;
		if( !(flags & gxCanvas::CANVAS_TEX_CUBE) ){
			if( w<=0 || h<=0 || first<0 || cnt<=0 ){
				w=h=first=0;
				if( gxCanvas *t=gx_graphics->loadCanvas( f,flags ) ){
					frames.push_back( t );
				}
				return;
			}
		}

		int t_flags=flags & (
			gxCanvas::CANVAS_TEX_RGB|
			gxCanvas::CANVAS_TEX_ALPHA|
			gxCanvas::CANVAS_TEX_MASK|
			gxCanvas::CANVAS_TEX_HICOLOR ) | gxCanvas::CANVAS_NONDISPLAY;

		gxCanvas *t=gx_graphics->loadCanvas( f,t_flags );
		if( !t ) return;
		if( !t->getDepth() ){
			gx_graphics->freeCanvas( t );
			return;
		}

		if( flags & gxCanvas::CANVAS_TEX_CUBE ){
			int w=t->getWidth()/6;
			if( w*6!=t->getWidth() ) return;
			int h=t->getHeight();

			gxCanvas *tex=gx_graphics->createCanvas( w,h,flags );
			if( tex ){
				frames.push_back( tex );

				for( int face=0;face<6;++face ){
					tex->setCubeFace(face);
					gx_graphics->copy( tex,0,0,tex->getWidth(),tex->getHeight(),t,face*w,0,w,h );
				}
				tex->setCubeFace(1);
			}
		}else{
			int x_tiles=t->getWidth()/w;
			int y_tiles=t->getHeight()/h;
			if( first+cnt>x_tiles*y_tiles ){
				gx_graphics->freeCanvas( t );
				return;
			}
			int x=(first%x_tiles)*w;
			int y=(first/x_tiles)*h;
			while( cnt-- ){
				gxCanvas *p=gx_graphics->createCanvas( w,h,flags );
				gx_graphics->copy( p,0,0,p->getWidth(),p->getHeight(),t,x,y,w,h );
				frames.push_back(p);
				x=x+w;if( x+w>t->getWidth() ){ x=0;y=y+h; }
			}
		}
		gx_graphics->freeCanvas( t );
	}

	~Rep(){
		--active_texs;
		for( int k=0;k<frames.size();++k ) gx_graphics->freeCanvas( frames[k] );
	}
};

CachedTexture::Rep *CachedTexture::findRep( String f,int flags,int w,int h,int first,int cnt ){
	std::set<Rep*>::const_iterator it;
	for( it=rep_set.begin();it!=rep_set.end();++it ){
		Rep *rep=*it;
		if( rep->file.equals(f) && rep->flags==flags && rep->w==w && rep->h==h && rep->first==first && rep->frames.size()==cnt ){
			++rep->ref_cnt;return rep;
		}
	}
	return 0;
}

CachedTexture::CachedTexture( int w,int h,int flags,int cnt ):
rep(new Rep(w,h,flags,cnt)){
}

CachedTexture::CachedTexture( String f_,int flags,int w,int h,int first,int cnt ){
	String f=f_;
	if( f.substr(0,2).equals(".\\") ) f=f.substr(2);
	if( path.size() ){
		String t=path+tolower( filenamefile( f ) );
		if( rep=findRep( t,flags,w,h,first,cnt ) ) return;
		rep=new Rep( t,flags,w,h,first,cnt );
		if( rep->frames.size() ){
			rep_set.insert( rep );
			return;
		}
		delete rep;
	}
	String t=tolower( fullfilename( f ) );
	if( rep=findRep( t,flags,w,h,first,cnt ) ) return;
	rep=new Rep( t,flags,w,h,first,cnt );
	rep_set.insert( rep );
}

CachedTexture::CachedTexture( const CachedTexture &t ):
rep(t.rep){
	++rep->ref_cnt;
}

CachedTexture::~CachedTexture(){
	if( !--rep->ref_cnt ){
		rep_set.erase( rep );
		delete rep;
	}
}

CachedTexture &CachedTexture::operator=( const CachedTexture &t ){
	++t.rep->ref_cnt;
	if( !--rep->ref_cnt ){
		rep_set.erase( rep );
		delete rep;
	}
	rep=t.rep;
	return *this;
}

String CachedTexture::getName()const{
	return rep->file;
}

const std::vector<gxCanvas*> &CachedTexture::getFrames()const{
	return rep->frames;
}

void CachedTexture::setPath( String t ){
	path=tolower(t);
	if( int sz=path.size() ){
		if( path.charAt(sz-1)!='/' && path.charAt(sz-1)!='\\' ) path=String(path,"\\");
	}
}
