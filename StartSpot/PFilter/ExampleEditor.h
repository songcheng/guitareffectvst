//	ExampleEditor.h - Simple OpenGL editor window.
//	--------------------------------------------------------------------------
//	Copyright (c) 2005-2006 Niall Moody
//	
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the "Software"),
//	to deal in the Software without restriction, including without limitation
//	the rights to use, copy, modify, merge, publish, distribute, sublicense,
//	and/or sell copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//	DEALINGS IN THE SOFTWARE.
//	--------------------------------------------------------------------------

#ifndef EXAMPLEEDITOR_H_
#define EXAMPLEEDITOR_H_

#include "VSTGLEditor.h"
#include "VSTGLTimer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class TextOption
{
public:
	TextOption(int x,int y,int w,int h, unsigned int r, unsigned int g, unsigned int b, unsigned int a, bool centerX=true, bool centerY=true)
		:x(x),y(y),w(w),h(h),centerX(centerX),centerY(centerY),r(r),g(g),b(b),a(a)
	{
	}
	int x,y,w,h;
	bool centerX;
	bool centerY;
	unsigned int r,g,b,a;
};
class QuadOption
{
public:
	QuadOption(int x,int y,int w,int h, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
		:x(x),y(y),w(w),h(h),r(r),g(g),b(b),a(a)
	{
	}
	int x,y,w,h;
	unsigned int r,g,b,a;
};

class QuadOptionBorder
{
public:
	QuadOptionBorder(int x,int y,int w,int h, unsigned int r, unsigned int g, unsigned int b, unsigned int a, unsigned int rb, unsigned int gb, unsigned int bb, unsigned int ab)
		:x(x),y(y),w(w),h(h),r(r),g(g),b(b),a(a),rb(rb),gb(gb),bb(bb),ab(ab)
	{
	}
	int x,y,w,h;
	unsigned int r,g,b,a;
	unsigned int rb,gb,bb,ab;
};
class GLGUI;
class GUIElement
{
public:
	GUIElement(int handle, GLGUI *gui);
	virtual void OnMouseDown(int button, int x, int y)
	{
	}
	virtual void OnMouseUp(int button, int x, int y)
	{
	}
	virtual void OnMouseMove(int x, int y)
	{
	}
	virtual void Draw()
	{
	}
	GLGUI *mGUI;
	int mHandle;
	bool hidden;
};
class SliderCallback
{
public:
	SliderCallback()
	{
	}
	virtual void SetVal(int val)
	{
	}
	virtual void OnRDown()
	{
	}
	
};
class ButtonCallback
{
public:
	ButtonCallback()
	{
	}
	virtual void OnClick()
	{
	}
};
class OnOffButtonCallback
{
public:
	OnOffButtonCallback()
	{
	}
	virtual void OnOn()
	{
	}
	virtual void OnOff()
	{
	}
};
class ListBoxCallback
{
public:
	ListBoxCallback()
	{
	}
	virtual void OnSelect(int idx)
	{
	}
};
class TabbedListBoxCallback
{
public:
	TabbedListBoxCallback()
	{
	}
	virtual void OnSelect(int idx)
	{
	}
	virtual void OnPageSelect(int idx)
	{
	}
};
class TabbedListBox;
class GLGUI
{
public:
	GLGUI();
	TabbedListBox *mPopupList;
	~GLGUI()
	{
		/*for(GUIElements::iterator i=mGUIElements.begin(); i != mGUIElements.end(); ++i)
		{
			delete (*i);
		}*/
		cleanup();
	}
	void AddElement(GUIElement *ele)
	{
		mGUIElements.push_back(ele);
	}
	GUIElement *GetElement(int handle)
	{
		for(GUIElements::iterator it=mGUIElements.begin(); it != mGUIElements.end(); ++it)
		{
			if((*it)->mHandle == handle)
			{
				return *it;
			}
		}
		return nullptr;
	}
	int GetNewHandle()
	{
		int result = handleCounter;
		handleCounter++;
		return result;
	}
	void Show(int handle, bool show)
	{
		for(GUIElements::iterator i=mGUIElements.begin(); i != mGUIElements.end(); ++i)
		{
			if((*i)->mHandle == handle)
			{
				(*i)->hidden = show;
				break;
			}
		}
	}
	int NewSlider(int x, int y, int w, int min_, int max_, bool isFreq=false);
	void SetSliderVal(int handle, int val);
	void SetSliderCallback(int handle, SliderCallback *cb);
	int NewList(int x,int y,int w,int h, ListBoxCallback *cb);
	void AddToList(int handle, string label, int idx=-1);
	void DeleteListItem(int handle, int idx);
	int NewTList(int x,int y,int w,int h, TabbedListBoxCallback *cb);
	void AddToTList(int handle, string label, int idx=-1);
	void DeleteTabbedListItem(int handle, int idx);
	int NewButton(int x, int y, int w, int h, string label, ButtonCallback *cb);
	int NewOnOffButton(int x, int y, int w, int h, string label, OnOffButtonCallback *cb);
	void DeleteGUIElement(int handle)
	{
		for(GUIElements::iterator i=mGUIElements.begin(); i != mGUIElements.end(); ++i)
		{
			if((*i)->mHandle == handle)
			{
				(*i)->hidden = true;
				GUIElement *item = *i;
				mGUIElements.erase(i);
				delete item;
				break;
			}
		}
		mGUIDeleted = true;
	}
	bool mGUIDeleted;
	int handleCounter;

	void DrawQuad(QuadOption &op);
	void DrawQuadBorder(QuadOptionBorder &op, int borderThick=1);
	void *m_hInstance;
	void init(HDC dc);
	void cleanup();
	GLvoid Print(TextOption &op, const char *fmt, ...);
	GLvoid Print2(TextOption &op, const char *fmt, ...);
	void DrawElements()
	{
		for(GUIElements::iterator i=mGUIElements.begin(); i != mGUIElements.end(); ++i)
		{
			if(!(*i)->hidden)
				(*i)->Draw();
		}
	}
	void onMouseDown(int button, int x, int y)
	{
		// GUIElements->OnMouseDown에서 mGUIElements자체를 수정하게 된다.
		// 그러므로 이 코드는 invalid
		// mGUIElements를 카피해서 써야한다.
		// 아 그래도 안된다.
		// 하나의 엘리먼트가 멀티플한 엘리먼트를 삭제하게 된다.
		// 어떻게 할지 고민좀 해보자.
		GUIElements copy;
		GUIElements alreadyUsed;
		for(int i=0; i < mGUIElements.size(); ++i)
		{
			copy.push_back(mGUIElements[i]);
		}
		int i=0;
		int max = copy.size();
		while(i < max)
		{
			bool found = false;
			for(int j=0; j< alreadyUsed.size(); ++j)
			{
				if(copy[i] == alreadyUsed[j])
					found = true;
			}
			if(!(copy[i])->hidden && !found)
				(copy[i])->OnMouseDown(button,x,y);
			alreadyUsed.push_back(copy[i]);
			i++;
			if(mGUIDeleted)
			{
				i = 0;
				copy.clear();
				for(int j=0; j < mGUIElements.size(); ++j)
				{
					copy.push_back(mGUIElements[j]);
				}
				mGUIDeleted = false;
				max = copy.size();
			}
		}
	}
	void onMouseUp(int button, int x, int y)
	{
		GUIElements copy;
		GUIElements alreadyUsed;

		for(int i=0; i < mGUIElements.size(); ++i)
		{
			copy.push_back(mGUIElements[i]);
		}
		int i=0;
		int max = copy.size();

		while(i < max)
		{
			bool found = false;
			for(int j=0; j< alreadyUsed.size(); ++j)
			{
				if(copy[i] == alreadyUsed[j])
					found = true;
			}
			if(!(copy[i])->hidden && !found)
				(copy[i])->OnMouseUp(button,x,y);
			alreadyUsed.push_back(copy[i]);

			i++;
			if(mGUIDeleted)
			{
				i = 0;
				copy.clear();
				for(int j=0; j < mGUIElements.size(); ++j)
				{
					copy.push_back(mGUIElements[j]);
				}
				mGUIDeleted = false;
				max = copy.size();
			}

		}
	}
	void onMouseMove(int x, int y)
	{
		GUIElements copy;
		GUIElements alreadyUsed;

		for(int i=0; i < mGUIElements.size(); ++i)
		{
			copy.push_back(mGUIElements[i]);
		}
		int i=0;
		int max = copy.size();
		while(i < max)
		{
			bool found = false;
			for(int j=0; j< alreadyUsed.size(); ++j)
			{
				if(copy[i] == alreadyUsed[j])
					found = true;
			}
			if(!(copy[i])->hidden && !found)
				(copy[i])->OnMouseMove(x,y);
			alreadyUsed.push_back(copy[i]);
			i++;
			if(mGUIDeleted)
			{
				i = 0;
				copy.clear();
				for(int j=0; j < mGUIElements.size(); ++j)
				{
					copy.push_back(mGUIElements[j]);
				}
				mGUIDeleted = false;
				max = copy.size();
			}

		}
	}
	typedef vector<GUIElement *> GUIElements;
	GUIElements mGUIElements;
private:
	GLuint image;
	GLuint mFont1;
	GLuint mFont2;

};

bool InRect(int x,int y,int w,int h,int x2,int y2);
class Slider : public GUIElement
{
public:
	Slider(int handle, GLGUI* gui, int x, int y, int w, int min_, int max_, bool isFreq=false)
		:GUIElement(handle, gui), 
		x(x),y(y),w(w),min_(min_),max_(max_),
		line(x+29, y+13/2, w-29, 3, 255,255,255,255, 0,0,0,255),
		slider(x+30, y, 20, 13, 255,255,255,255, 0,0,0,255),
		top(x,y,29,13, 0,0,0,255), mIsFreq(isFreq)

	{
		h = 13;
		LMouseDown = false;
		mCB = nullptr;
	}
	bool mIsFreq;
	SliderCallback *mCB;
	void SetCallback(SliderCallback *cb)
	{
		mCB = cb;
	}

	void Draw()
	{
		mGUI->Print(top, "%d", curVal);
		mGUI->DrawQuadBorder(line);
		mGUI->DrawQuadBorder(slider);
	}
	void OnMouseDown(int button, int x_, int y_)
	{
		if(button == 1 && InRect(slider.x,slider.y,slider.w,slider.h,x_,y_))
		{
			clickedX = x_;
			clickedY = y_;
			LMouseDown = true;
		}
		else if(button == 2 && InRect(x,y,w,h, x_, y_))
		{
			if(mCB) mCB->OnRDown();
		}
	}
	int clickedX, clickedY;
	void OnMouseUp(int button, int x_, int y_)
	{
		if(button == 1) LMouseDown = false;
	}

	void OnMouseMove(int x_, int y_)
	{
		if(LMouseDown)
		{
			slider.x += x_-clickedX;
			clickedX = x_;
			if(slider.x < x+30)
				slider.x = x+30;
			if(slider.x > x+w-1-20)
				slider.x = x+w-1-20;
			int prevVal = curVal;
			UpdateVal();
			if(prevVal != curVal && mCB)
				mCB->SetVal(curVal);
		}
	}
	void UpdateVal()
	{
		int startPosX = x+30;
		int val = slider.x - startPosX;
		float valFloat = (float)val / (float)CalculateTotalSliderLength();
		curVal = (int)((float)min_ + float(max_-min_)*valFloat);
	}
	void SetVal(int val)
	{
		int prevVal = curVal;
		curVal = val;
		if(prevVal != curVal && mCB) mCB->SetVal(curVal);
		SetPosByVal();
	}
	void SetPosByVal()
	{
		int startPosX = x+30;

		float valFloat = float(curVal-min_) / (float)((max_-min_));
		int val = (int)(valFloat * (float)CalculateTotalSliderLength());
		slider.x = val + startPosX;
	}
	int CalculateTotalSliderLength()
	{
		return line.w-slider.w-2;
	}
	bool LMouseDown;

	TextOption top;
	QuadOptionBorder line;
	QuadOptionBorder slider;

	int x,y,w,h,min_,max_;
	int curVal;
};


class Button : public GUIElement
{
public:
	Button(int handle, GLGUI* gui, int x, int y, int w, int h, string label)
		:GUIElement(handle, gui), 
		x(x),y(y),w(w),h(h), mLabel(label),
		top(x,y,w,h, 0,0,0,255),
		button(x,y,w,h, 255,255,255,255, 0,0,0,255)

	{
		mCB = nullptr;
	}
	string mLabel;
	ButtonCallback *mCB;
	void SetCallback(ButtonCallback *cb)
	{
		mCB = cb;
	}

	void Draw()
	{
		mGUI->DrawQuadBorder(button);
		mGUI->Print(top, "%s", mLabel.c_str());
	}
	void OnMouseDown(int button, int x_, int y_)
	{
		if(button == 1 && InRect(x,y,w,h,x_,y_))
		{
			if(mCB) mCB->OnClick();
		}
	}
	TextOption top;
	QuadOptionBorder button;

	int x,y,w,h;
};

class ListBox : public GUIElement
{
public:
	ListBox(int handle, GLGUI* gui, int x, int y, int w, int h)
		:GUIElement(handle, gui), 
		x(x),y(y),w(w),h(h),
		box(x,y,w,h, 255,255,255,255, 0,0,0,255)

	{
		mCB = nullptr;
		selected = -1;
		listH = 15;
	}
	ListBoxCallback *mCB;
	void MoveUpSelection()
	{
		if(selected >= 1 && selected < mStrs.size())
		{
			string backup = mStrs[selected-1];
			mStrs[selected-1] = mStrs[selected];
			mStrs[selected] = backup;
			selected -= 1;
		}
	}
	void MoveDnSelection()
	{
		if(selected >= 0 && selected < mStrs.size()-1)
		{
			string backup = mStrs[selected+1];
			mStrs[selected+1] = mStrs[selected];
			mStrs[selected] = backup;
			selected += 1;
		}
	}
	void SetCallback(ListBoxCallback *cb)
	{
		mCB = cb;
	}
	vector<string> mStrs;
	void Add(string label, int idx = -1)
	{
		if(idx != -1)
			mStrs.insert(mStrs.begin()+idx, label);
		else
			mStrs.push_back(label);
	}
	void Delete(int idx)
	{
		if(idx >= 0 && idx < mStrs.size())
			mStrs.erase(mStrs.begin()+idx);
	}
	int selected;
	int listH;
	void Draw()
	{
		mGUI->DrawQuadBorder(box);
		int curY = y;
		int idx=0;
		for(vector<string>::iterator it = mStrs.begin(); it != mStrs.end(); ++it)
		{
			if(curY + listH > y+h) break;
			TextOption top(x,curY,w,listH, 0,0,0,255);
			if(idx == selected)
			{
				QuadOptionBorder sel(x,curY,w,listH, 32,128,32,255,  0,0,0,255);
				mGUI->DrawQuadBorder(sel);
			}
			curY += listH;
			idx++;
			mGUI->Print(top, "%s", (*it).c_str());
		}
		
	}
	void OnMouseDown(int button, int x_, int y_)
	{
		if(button == 1)
		{
			int curY = y;
			int idx=0;
			bool found = false;
			for(vector<string>::iterator it = mStrs.begin(); it != mStrs.end(); ++it)
			{
				if(curY + listH > y+h) break;
				if(InRect(x,curY,w,listH,x_,y_))
				{
					selected = idx;
					found = true;
				}
				idx++;
				curY += listH;
			}
			if(found && mCB) mCB->OnSelect(selected);
		}
	}
	QuadOptionBorder box;

	int x,y,w,h;
};

class TabbedListBox : public GUIElement
{
public:
	TabbedListBox(int handle, GLGUI* gui, int x, int y, int w, int h)
		:GUIElement(handle, gui), 
		x(x),y(y),w(w),h(h),
		box(x,y,w,h, 255,255,255,255, 0,0,0,255)

	{
		mCB = nullptr;
		selected = -1;
		listH = 15;
		buttonH = 20;
		margin = 3;
		curPage = 0;
	}
	int buttonH;
	int margin;
	TabbedListBoxCallback *mCB;
	void SetCallback(TabbedListBoxCallback *cb)
	{
		mCB = cb;
	}
	vector<string> mStrs;
	void Clear()
	{
		mStrs.clear();
		selected = -1;
		curPage = 0;
	}
	void Add(string label, int idx = -1)
	{
		if(idx != -1)
			mStrs.insert(mStrs.begin()+idx, label);
		else
			mStrs.push_back(label);
	}
	int curPage;
	void Delete(int idx)
	{
		if(idx >= 0 && idx < mStrs.size())
			mStrs.erase(mStrs.begin()+idx);
	}
	int selected;
	int listH;
	int GetNumItemsPerPage()
	{
		return (h-margin*2-buttonH)/listH;
	}
	int GetNumButtons()
	{
		int numItemPerPage = (h-margin*2-buttonH)/listH;
		return (mStrs.size()/numItemPerPage)+1;
	}
	void Draw()
	{
		mGUI->DrawQuadBorder(box);
		QuadOptionBorder boxList(x,y,w,h-margin*2-buttonH, 255,255,255,255, 0,0,0,255);
		mGUI->DrawQuadBorder(boxList);

		for(int i=0; i<GetNumButtons();++i)
		{
			QuadOptionBorder button(x+margin + i*margin + i*buttonH,y+h-margin-buttonH,buttonH, buttonH, 255,255,255,255, 0,0,0,255);
			mGUI->DrawQuadBorder(button);
			TextOption top2(x+margin + i*margin + i*buttonH,y+h-margin-buttonH,buttonH, buttonH, 0,0,0,255);
			mGUI->Print(top2, "%d", i+1);
		}
		int curY = y;
		int idx=0;
		char temp[123];
		sprintf(temp, "%d %d %d", curPage, GetNumItemsPerPage(), mStrs.size());
		//MessageBox(NULL, temp, temp, MB_OK);
		if(curPage*GetNumItemsPerPage() >= 0 && curPage*GetNumItemsPerPage() < mStrs.size())
		{
			for(vector<string>::iterator it = mStrs.begin()+curPage*GetNumItemsPerPage(); it != mStrs.end(); ++it)
			{
				if(curY + listH > y+h-margin*2-buttonH) break;
				TextOption top(x,curY,w,listH, 0,0,0,255);
				if(idx+curPage*GetNumItemsPerPage() == selected)
				{
					QuadOptionBorder sel(x,curY,w,listH, 32,128,32,255,  0,0,0,255);
					mGUI->DrawQuadBorder(sel);
				}
				curY += listH;
				idx++;
				mGUI->Print(top, "%s", (*it).c_str());
			}
		}
		
	}
	void OnMouseDown(int button, int x_, int y_)
	{
		if(button == 1)
		{
			int curY = y;
			int idx=0;
			bool found = false;
			for(vector<string>::iterator it = mStrs.begin()+curPage*GetNumItemsPerPage(); it != mStrs.end(); ++it)
			{
				if(curY + listH > y+h-margin*2-buttonH) break;
				if(InRect(x,curY,w,listH,x_,y_))
				{
					selected = idx+curPage*GetNumItemsPerPage();
					found = true;
					break;
				}
				idx++;
				curY += listH;
			}
			if(found && mCB) mCB->OnSelect(selected);
			
			for(int i=0; i<GetNumButtons();++i)
			{
				if(InRect(x+margin + i*margin + i*buttonH,y+h-margin-buttonH,buttonH, buttonH,x_,y_))
				{
					curPage = i;
					if(mCB) mCB->OnPageSelect(curPage);
					selected = -1;
					if(mCB) mCB->OnSelect(selected);
					break;
				}
			}


		}
	}
	QuadOptionBorder box;

	int x,y,w,h;
};

class OnOffButton : public GUIElement
{
public:
	OnOffButton(int handle, GLGUI* gui, int x, int y, int w, int h, string label)
		:GUIElement(handle, gui), 
		x(x),y(y),w(w),h(h), mLabel(label),
		top(x+17,y,w-17,h, 0,0,0,255),
		button(x,y,w,h, 255,255,255,255, 0,0,0,255),
		indicatorOn(x+14/2-10/2, y+h/2-10/2, 10, 10, 0, 255, 0, 255,  0,0,0,255),
		indicatorOff(x+14/2-10/2, y+h/2-10/2, 10, 10, 0, 0, 0, 255,  0,0,0,255)

	{
		mOn = false;
		mCB = nullptr;
	}
	bool mOn;
	string mLabel;
	
	OnOffButtonCallback *mCB;
	void SetCallback(OnOffButtonCallback *cb)
	{
		mCB = cb;
	}

	void Draw()
	{
		mGUI->DrawQuadBorder(button);
		if(mOn)
			mGUI->DrawQuadBorder(indicatorOn);
		else
			mGUI->DrawQuadBorder(indicatorOff);
		mGUI->Print(top, "%s", mLabel.c_str());
	}
	void OnMouseDown(int button, int x_, int y_)
	{
		if(button == 1 && InRect(x,y,w,h,x_,y_))
		{
			mOn = !mOn;
			if(mCB) 
			{
				if(mOn)
					mCB->OnOn();
				else
					mCB->OnOff();
			}
		}
	}
	void SetOn(bool onOff)
	{
		mOn = onOff;
		if(mCB) 
		{
			if(mOn)
				mCB->OnOn();
			else
				mCB->OnOff();
		}
	}
	TextOption top;
	QuadOptionBorder button;
	QuadOptionBorder indicatorOn;
	QuadOptionBorder indicatorOff;

	int x,y,w,h;
};

///	Simple VSTGL example.
class ExampleEditor : public VSTGLEditor,
					  public Timer
{
  public:
	///	Constructor.
	ExampleEditor(AudioEffect *effect);
	///	Destructor.
	~ExampleEditor();

	///	Called when the Gui's window is opened.
	void guiOpen();
	///	Called when the Gui's window is closed.
	void guiClose();

	///	Draws a spinning pyramid.
	void draw();

	///	Called repeatedly, to update the graphics.
	void timerCallback();
	GLGUI *mGUI;
  private:
	///	Variable used to rotate the pyramid.
	float thing;
};

#endif
