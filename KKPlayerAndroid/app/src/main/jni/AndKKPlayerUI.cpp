#include "AndKKPlayerUI.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
static const char *gVertexShader=  "attribute vec4 vPosition;\n \
                void main() {\n \
                gl_Position = vPosition;\n \
                }\n";

static const char *gFragmentShader =
        "precision mediump float;\n \
        uniform sampler2D u_Texture;\n \
        varying lowp vec2 v_TexCoordinate;\n \
        void main() {\n \
        gl_FragColor =texture2D(u_Texture,v_TexCoordinate);\n \
        }\n";

static void printGLString(const char *name, GLenum s)
{
    const char *v = (const char *)::glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
CAndKKPlayerUI::CAndKKPlayerUI():m_player(this,&m_Audio)
{
    //播放器未启动
    m_playerState=-1;
    m_pGLHandle=0;
    gvPositionHandle=0;
    m_Screen_Width=0;
    m_Screen_Height=0;
    m_nTextureID=0;
    m_bAdJust= false;
    m_player.InitSound();
    m_player.SetWindowHwnd(0);
    m_bNeedReconnect= false;
}
CAndKKPlayerUI::~CAndKKPlayerUI()
{
    if(m_nTextureID!=0)
    {
        glDeleteTextures(2,&m_nTextureID);
        m_nTextureID=0;
    }
    m_player.CloseMedia();
    m_Audio.CloseAudio();
    LOGI("~CAndKKPlayerUI\n");
}
void  CAndKKPlayerUI::Pause()
{
    m_player.Pause();
}
void  CAndKKPlayerUI::Seek(int value)
{
    m_player.KKSeek(Left,value);
}
int CAndKKPlayerUI::IniGl()
{
    //播放器启动
    //m_playerState=1;
    //m_player.OpenMedia("rtmp://live.hkstv.hk.lxdns.com/live/hks live=1");
    //m_player.OpenMedia("/storage/emulated/0/Android/aaa.flv");
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    // 启用阴影平滑
    glShadeModel(GL_SMOOTH);
    // 黑色背景
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 设置深度缓存
    glClearDepthf(1.0f);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 所作深度测试的类型
    glDepthFunc(GL_LEQUAL);

    // 对透视进行修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    return m_pGLHandle;
}
MEDIA_INFO CAndKKPlayerUI::GetMediaInfo()
{
    return m_player.GetMediaInfo();
}
int  CAndKKPlayerUI::OpenMedia(char *str)
{
    LOGI(" CAndKKPlayerUI %s,%d\n",str, m_playerState);
    if( m_playerState<=-1)
    {
        m_bNeedReconnect=false;
        m_player.CloseMedia();
        m_playerState=m_player.OpenMedia(str);
    }
    return m_playerState;
}
// 定义π
const GLfloat PI = 3.1415f;
static void _gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat top = zNear * ((GLfloat) tan(fovy * PI / 360.0));
    GLfloat bottom = -top;
    GLfloat left = bottom * aspect;
    GLfloat right = top * aspect;
    glFrustumf(left, right, bottom, top, zNear, zFar);
}
int CAndKKPlayerUI::Resizeint(int w,int h)
{
    m_width=w;
    m_height=h;
    // 重置当前的视口
    glViewport(0, 0,w, h);
    // 选择投影矩阵
    glMatrixMode(GL_PROJECTION);//对投影矩阵应用随后的矩阵操作.
    // 重置投影矩阵
    glLoadIdentity();

    // 设置视口的大小
   // _gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,-10.0f,100.0f);

    // 选择模型观察矩阵
    glMatrixMode(GL_MODELVIEW);//对模型视景矩阵堆栈应用随后的矩阵操作.

    // 重置模型观察矩阵
    glLoadIdentity();//:将当前的用户坐标系的原点移到了屏幕中心：类似于一个复位操作

    //m_player.AdjustDisplay(w,h);
    m_bAdJust= false;
}

//xyz
const GLfloat gVertices[] = {
        -1.0f, -1.0f, 0.0f, // 左下
        1.0f, -1.0f, 0.0f,  // 右下
        -1.0f, 1.0f, 0.0f,  // 左上
        1.0f, 1.0f, 0.0f    // 右上
};

const GLfloat gTextureCoord[] = {
        0.0f,1.0f,
        1.0f,1.0f,
        0.0f,0.0f,
        1.0f,0.0f,
};
GLfloat gVertices2[] = {
-1.0f, -1.0f, 0.0f, // 左下
1.0f, -1.0f, 0.0f,  // 右下
-1.0f, 1.0f, 0.0f,  // 左上
1.0f, 1.0f, 0.0f    // 右上
};
void CAndKKPlayerUI::renderFrame()
{
    // 清除屏幕及深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 设置背景颜色为黑色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // 重置当前的模型观察矩阵,将当前的用户坐标系的原点移到了屏幕中心
    glLoadIdentity();
    // 启用顶点数组
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // 纹理设置
    glEnable(GL_TEXTURE_2D);                                // 启用纹理映射
        // 选择纹理

   // LOGI(" renderFrame %d\n",0);
    m_player.RenderImage(this, false);
    if(m_nTextureID==0)
        return;

    glBindTexture(GL_TEXTURE_2D, m_nTextureID);              // 选择纹
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);            // 启用纹理坐标数组



    //if(!m_bAdJust)
    {
        float w2=m_width;
        float h2=(float)(m_Picheight*w2)/m_Picwidth;
        if(h2>m_height)
        {
            h2=m_height;
            w2=(float)(m_Picwidth*h2)/m_Picwidth;
        }
        float w= (float)w2/m_width;
        float h= (float)h2/m_height;
        gVertices2[0]= -w;
        gVertices2[1]=-h;
        gVertices2[2]=0.0f; // 左下

        gVertices2[3]=w;
        gVertices2[4]=-h;
        gVertices2[5]=0.0f;  // 右下

        gVertices2[6]=-w;
        gVertices2[7]=h;
        gVertices2[8]=0.0f;  // 左上

        gVertices2[9]=w;
        gVertices2[10]=h;
        gVertices2[11]=0.0f;    // 右上
        m_bAdJust=true;
    }
     // 绘制正方形
      glTranslatef(0.0f,0.0f,-1.0f);                         // 设置三角形位置
      glVertexPointer(3, GL_FLOAT, 0, gVertices2);             // 指定顶点数组
      glTexCoordPointer(2, GL_FLOAT, 0, gTextureCoord);       // 设置纹理坐标
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);                      // 绘制三角形

    // 关闭顶点数组
    glDisableClientState(GL_VERTEX_ARRAY);
    // 关闭纹理数组
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    return;
}
bool CAndKKPlayerUI::GetNeedReconnect()
{
    return  m_bNeedReconnect;
}
int CAndKKPlayerUI::GetPlayerState()
{
    return m_playerState;
}
unsigned char* CAndKKPlayerUI::GetWaitImage(int &length,int curtime)
{
    return NULL;
}
unsigned char*  CAndKKPlayerUI::GetCenterLogoImage(int &length)
{
    return NULL;
}
void CAndKKPlayerUI::LoadCenterLogo(unsigned char* buf,int len)
{

}
unsigned char* CAndKKPlayerUI::GetBkImage(int &length)
{
    return NULL;
}
void CAndKKPlayerUI::OpenMediaFailure(char *strURL)
{
    LOGE("Open Err");
    m_playerState=-2;
    return;
}
//得到延迟
int  CAndKKPlayerUI::GetRealtimeDelay()
{
    return m_player.GetRealtimeDelay();
}
//强制刷新Que
void  CAndKKPlayerUI::ForceFlushQue()
{
    m_player.ForceFlushQue();
}
void  CAndKKPlayerUI::AutoMediaCose(int Stata)
{
     if(Stata==-1)
     {
         m_bNeedReconnect=true;
     }
    m_playerState=-3;
}
bool CAndKKPlayerUI::init(HWND hView)
{

}
int  CAndKKPlayerUI::CloseMedia()
{
    m_player.CloseMedia();
    m_playerState=-1;
    return 0;
}
void CAndKKPlayerUI::destroy()
{

}
void CAndKKPlayerUI::resize(unsigned int w, unsigned int h)
{

}
void CAndKKPlayerUI::WinSize(unsigned int w, unsigned int h)
{

}


void  CAndKKPlayerUI::AVRender()
{
    //LOGI(" renderFrame %d\n",1);
   // m_player.RenderImage(this, false);
}
void CAndKKPlayerUI::render(char* buf,int width,int height)
{
    //LOGI(" renderFrame %d\n",2);
   // m_RenderLock.Lock();
    if(buf!=NULL)
    {
        if(m_nTextureID==0)
        {
            glGenTextures(2, &m_nTextureID);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_nTextureID );
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width,height,0,GL_RGB,GL_UNSIGNED_BYTE,buf);
        m_Picwidth=width;
        m_Picheight=height;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
   // m_RenderLock.Unlock();
}
//呈现背景图片
void CAndKKPlayerUI::renderBk(unsigned char* buf,int len){}
void CAndKKPlayerUI::SetWaitPic(unsigned char* buf,int len){}
void CAndKKPlayerUI::SetBkImagePic(unsigned char* buf,int len){}
int CAndKKPlayerUI::GetIsReady()
{
    return  m_player.GetIsReady();
}
int CAndKKPlayerUI::GetRealtime()
{
    return  m_player.GetRealtime();
}
