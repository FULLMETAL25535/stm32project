#include "stm32f10x.h"                  //                                                       实现功能：修改密码
#include "Delay.h"						//延迟函数											   密码掉电不丢失（冻结状态也是）
#include "OLED.h"                       //OLED显示函数   接线 B8-SCL  B9-SDA							 开关密码锁 
#include "KEY.h"					    //按键PA0-PA11   PA0-PA10为数字0-9 PA11为确定					 显示图片
#include "Store.h"						//密码掉电不丢失 包括冻结(doge)
int main() {
	Key_Init_A();					    //KEY初始化
	Key_Init_B();						//KEY初始化
	OLED_Init();
	Store_Init();
//	Store_Data[1]=12345;           防止忘记密码随时可以改你想要的密码
//	Store_Save();				   只需要你解除注释然后下载进去，密码就是12345
//	Store_Data[2]=1;			   //同理解除冻结           注意：解除注释录进去后冻结解除，如果需要加入冻结功能
//	Store_Save();					//						需要恢复注释再录一遍(试了一个多小时才发现原来是这个问题)
	int KeyNum;					        //键码
	int last;							//错误次数标志位
	unsigned int Sum=0;                 //输入的密码
	int password=12345;					//初始密码
	int Show_Num=40;					//OLED显示
	int Change=3;						//
	password=Store_Data[1];                     //读取flash里的内容
	last=Store_Data[2];
	OLED_Update();
	OLED_ShowImage(65, 0, 60, 60,Citrus_1);     //用来显示作者Werner头像 QVQ
	OLED_ShowImage(0, 0, 60, 60,Star);
	OLED_Update();
	Delay_ms(4000);
	OLED_Clear();  
	int enter=1;                                

//  last=1;                                    解除冻结
	while(1) {
		if(last==3) {                           //判断上次开机是否处于冻结状态
			OLED_Clear();
			OLED_ShowChinese(0, 0, "由于你失误过多了");
			OLED_ShowChinese(0, 16, "您的账号已冻结！！！");
			OLED_ShowChinese(0, 32, "请。。。秒后再试");
			OLED_ShowNum(16, 32,11451, 5, OLED_8X16);
			OLED_Update();
			Delay_ms(1145141);
		}

start:
		if(enter%2==0) {                  							//关闭密码锁后不需要再次输入密码       
			OLED_Clear();
			OLED_ShowChinese(0, 16, "由于关闭了密码锁");
			OLED_ShowChinese(20, 32, "即将进入系统");
			OLED_ShowChinese(60, 48, "索");
			OLED_Update();
			Delay_ms(2000);
		}
		OLED_Clear();
		Show_Num=40;
		Sum=0;
		while(enter%2==1&&Change!=0) {
			OLED_ShowChinese(0, 0, "请输入密码：");				    
			OLED_ShowChinese(0, 16, "密码：");
			OLED_ShowChinese(60, 48, "嗦");							//16*16的一个关了的锁的图像
			OLED_Update();
			KeyNum=Key_GetNum();									//获取键码
			if(KeyNum) {
				if(KeyNum==10) {
					KeyNum=0;
				}
				if(KeyNum>=0&&KeyNum<=10) {							
					Show_Num+=8;									//显示右移8个格子（数字占8个像素）
					Sum=Sum*10+KeyNum;								//通过累乘实现保存输入的值
					OLED_ShowNum(Show_Num, 16,KeyNum, 1, OLED_8X16);
					OLED_Update();
				}
				if(KeyNum==11) {

					if(password==Sum) {                       		//如果密码正确显示 "O"      
						OLED_Clear();
						OLED_DrawCircle(63,30, 30, OLED_UNFILLED);
						OLED_DrawCircle(63,30, 29, OLED_UNFILLED);
						OLED_DrawCircle(63,30, 28, OLED_UNFILLED);
						OLED_Update();
						Delay_ms(2000);
						OLED_Clear();
						Sum=0;
						Show_Num=40;
						break;
					} else {
						Change--;
						OLED_Clear();								//如果密码正确显示 "X" 
						OLED_DrawLine(0, 0, 127, 63);
						OLED_DrawLine(1, 0, 126, 62);
						OLED_DrawLine(2, 0, 125, 61);
						OLED_DrawLine(0, 63, 127, 0);
						OLED_DrawLine(1, 63, 126, 0);
						OLED_DrawLine(2, 63, 125, 0);
						OLED_Update();
						Delay_ms(1000);
						OLED_Clear();
						if(Change!=0) {
							OLED_ShowChinese(10, 30, "你还有，次机会");             //中文显示没有数字故用图像覆盖法
							OLED_ShowNum(60, 30,Change, 1, OLED_8X16);				//显示change--数字
							OLED_Update();
							Delay_ms(1000);
							OLED_Clear();
						}
						if(Change==0) {
							last++;
							if(last==3) {											//冻结账号
								OLED_Clear();
								OLED_ShowChinese(0, 0, "由于你失误过多了");
								OLED_ShowChinese(0, 16, "您的账号已冻结！！！");
								OLED_ShowChinese(0, 32, "请。。。秒后再试");
								OLED_ShowNum(16, 32,11451, 5, OLED_8X16);					
								Store_Data[2]=last;								    //存入flash数组
								Store_Save();
								OLED_Update();
								Delay_ms(1145141);
							}
							OLED_Clear();
							OLED_ShowChinese(0, 16, "由于你失误太多了");
							OLED_ShowChinese(0, 32, "请稍后再试");
							OLED_ShowImage(80, 36, 16, 16,Smile);				    //显示笑脸
							OLED_Update();

							Delay_ms(5000*last);									//错误次数越多重试时间越长
							OLED_Clear();
							Change=3;
						}
					}
					Sum=0;
					Show_Num=40;
				}
			}
		}




//菜单栏
		while(1) {
choose:
			OLED_Clear();													//选择菜单
			OLED_ShowChinese(0, 0,"请选择以下功能：\n");
			OLED_ShowChinese(0, 16,"一、关闭或开启密码锁");
			OLED_ShowChinese(0, 32,"二、修改密码。");
			OLED_ShowChinese(0, 48,"三、保存并重启");
			OLED_Update();

			KeyNum=Key_GetNum();
			if(KeyNum) {
				if(KeyNum>3) {											    //选择菜单以外的会显示错误
					OLED_Clear();
					OLED_ShowChinese(20, 16,"输入错误");
					OLED_ShowChinese(20, 32,"请重新输入");
					OLED_Update();
					Delay_ms(1000);
				}
				if(KeyNum>=1&&KeyNum<=3) {
					switch(KeyNum) {
						case 1:												//开关密码锁
							if(enter%2==1) {								//显示关锁动画效果
								OLED_Clear();
								OLED_ShowChinese(10, 32, "嗦");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(26, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(42, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(58, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(74, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(90, 32, "索");
								OLED_Update();
								Delay_ms(500);
								OLED_Clear();
								OLED_ShowChinese(10, 16,"关闭密码锁成功");
								OLED_ShowChinese(0, 32,"自动跳回设置界面");
								OLED_Update();
								Delay_ms(3000);
								OLED_Clear();
							}
							if(enter%2==0) {  							    //显示开锁动画效果              
								OLED_Clear();
								OLED_ShowChinese(10, 32, "索");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(26, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(42, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(58, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(74, 32, "杠");
								OLED_Update();
								Delay_ms(500);
								OLED_ShowChinese(90, 32, "嗦");
								OLED_Update();
								Delay_ms(500);
								OLED_Clear();
								OLED_ShowChinese(10, 16,"开启密码锁成功");
								OLED_ShowChinese(0, 32,"自动跳回设置界面");
								OLED_Update();
								Delay_ms(3000);
								OLED_Clear();
							}
							enter++;
							break;
						case 2:												  //修改密码部分
							OLED_Clear();
							while(1) {
								OLED_ShowChinese(0, 0, "请输入原密码：");
								OLED_ShowChinese(0, 16, "密码：");
								OLED_Update();
								KeyNum=Key_GetNum();
								if(KeyNum) {
									if(KeyNum==10) {						//输入密码如果是10则为0
										KeyNum=0;
									}
									if(KeyNum>=0&&KeyNum<=10) {             //密码位数保存
										Show_Num+=8;
										Sum=Sum*10+KeyNum;
										OLED_ShowNum(Show_Num, 16,KeyNum, 1, OLED_8X16);
										OLED_Update();
									}
									if(KeyNum==11) {
										Show_Num=40;
										if(Sum!=password) {					//密码不对显示×
											OLED_Clear();
											OLED_DrawLine(0, 0, 127, 63);
											OLED_DrawLine(1, 0, 126, 62);
											OLED_DrawLine(2, 0, 125, 61);
											OLED_DrawLine(0, 63, 127, 0);
											OLED_DrawLine(1, 63, 126, 0);
											OLED_DrawLine(2, 63, 125, 0);
											OLED_Update();
											Delay_ms(1000);
											OLED_Clear();
											Sum=0;
											break;
										}
										if(Sum==password) {
											Sum=0;
											Show_Num=40;
											OLED_Clear();
											OLED_DrawCircle(63,30, 30, OLED_UNFILLED);
											OLED_DrawCircle(63,30, 29, OLED_UNFILLED);
											OLED_DrawCircle(63,30, 28, OLED_UNFILLED);
											OLED_Update();
											Delay_ms(2000);
											OLED_Clear();
											while(1) {
												OLED_ShowChinese(0, 0, "请修改密码：");
												OLED_ShowChinese(0, 16, "密码：");
												OLED_Update();
												KeyNum=Key_GetNum();
												if(KeyNum) {
													if(KeyNum==10) {
														KeyNum=0;
													}
													if(KeyNum>=0&&KeyNum<=10) {
														Show_Num+=8;
														Sum=Sum*10+KeyNum;
														OLED_ShowNum(Show_Num, 16,KeyNum, 1, OLED_8X16);
														OLED_Update();
													}


													if(KeyNum==11) {
														password=Sum;					//修改原密码保存到password
														OLED_Clear();
														OLED_ShowChinese(20, 0, "修改成功！");
														OLED_ShowImage(20, 20, 60, 60,Change_lock);
														OLED_Update();
														Store_Data[1]=password;         //保存到闪存数组
														Store_Save();
														Delay_ms(2000);
														goto choose;
													}
												}

											}

										}


									}

								}


							}



							break;
						case 3:
							last=1;											//保存并重启则回到初始阶段
							Change=3;
							goto start;
							break;
					}


				}


			}



		}









	}


}
//参考本人之前C语言实现的密码锁代码
//int main() {
//    int enter = 0;
//    int password = 123456;
//    int code = password;
//    while(1) {
//        if(enter==-1) {
//            printf("由于关闭了密码锁现在进入界面系统\n");
//            goto out;
//        } else if(enter!=-1) {
//            int n = 0;
//            int cnt = 2;
//            printf("-------------请输入密码-------------\n");            //进入系统输入密码程序
//            while(cnt>-1) {
//                printf("密码:");
//                scanf("%d",&n);
//                if(n==password) {
//                    printf("密码正确！您已进入系统界面\n");
//                    break;
//                } else {
//                    if(cnt==0) {
//                        printf("密码错误！您的账号已冻结！");
//                        return 0;
//                    }
//                    printf("密码错误！请重新输入，你还有%d次机会\n",cnt);
//                    cnt--;
//                }
//            }
//        }
//out:
//        ;
//        printf("----------密码系统界面-------\n");                     //设置界面
//        printf("------------1.设置------------\n");
//        printf("请输入对应的菜单\n");
//        int a=0;
//        while(1) {
//            scanf("%d",&a);
//            if(a==1) {
//                break;
//            } else {
//                printf("输入有误，再试一遍\n");
//            }
//        }
//        while(1) {
//            printf("----------------设置界面---------------\n");
//            printf("-------------1.关闭密码锁--------------\n");
//            printf("-------------2.开启密码锁--------------\n");
//            printf("-------------3.修改密码----------------\n");
//            printf("-------------4.保存并重启系统----------\n");
//            printf("-------------5.查看加密操作后的密码----\n");
//            printf("请输入对应的菜单\n");
//            int type;
//            scanf("%d",&type);
//            int cnt = 2;
//            int n = 0;
//            switch(type) {
//                case 1:
//                    printf("----------关闭密码锁------------\n");
//                    enter = -1;
//                    printf("关闭密码锁设置成功\n");
//                    printf("自动跳回设置界面\n\n");
//                    break;
//                case 2:
//                    printf("----------2.开启密码锁------------\n");
//                    enter = 0;
//                    printf("开启密码锁成功\n");
//                    printf("自动跳回设置界面\n\n");
//                    break;
//                case 3:
//                    printf("----------3.修改密码--------------\n");
//                    printf("----------请输入原密码--------\n");
//                    while(cnt>-1) {
//                        printf("原密码:");
//                        scanf("%d",&n);
//                        if(n==password) {
//                            printf("密码正确！现在你可以修改密码了！\n");
//                            scanf("%d",&password);
//                            code = password;
//                            printf("密码修改成功，现在跳回设置界面\n");
//                            break;
//                        } else {
//                            if(cnt==0) {
//                                printf("密码错误！现在跳回设置界面\n");
//                                break;
//                            }
//                            printf("密码错误！请重新输入，你还有%d次机会\n",cnt);
//                            cnt--;
//                        }
//                    }
//                    break;
//                case 4:
//                    printf("----------保存并重启系统--------\n");
//                    printf("保存并重启成功！现在跳回初始界面---\n");
//                    goto end;
//                    break;
//                case 5:
//                    printf("----------查看加密操作后的密码--------\n");
//                    printf("以下是加密后的密码:");
//                    int i=0;
//                    int number[20];
//                    char str[20];                                               //编码
//                    sprintf(str,"%d",code);
//                    int len = strlen(str);
//                    for(i=0;i<len; i++) {
//                        number[i] = str[i]+5;
//                    }
//                    i=0;
//                    printf("\n");
//                    for(i=0;i<len; i++) {
//                        binary(number[i]);
//                    }
//                    printf("\n");
//                    break;

//            }
//        }
//end:;
//    }
//}
//————————————————
//版权声明：本文为CSDN博主「Citrus-1120」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/2301_79578379?type=blog

//	/*OLED初始化*/

//
//	/*在(0, 0)位置显示字符'A'，字体大小为8*16点阵*/
//	OLED_ShowChar(0, 0, 'A', OLED_8X16);
//
//	/*在(16, 0)位置显示字符串"Hello World!"，字体大小为8*16点阵*/
//	OLED_ShowString(16, 0, "Hello World!", OLED_8X16);
//
//	/*在(0, 18)位置显示字符'A'，字体大小为6*8点阵*/
//	OLED_ShowChar(0, 18, 'A', OLED_6X8);
//
//	/*在(16, 18)位置显示字符串"Hello World!"，字体大小为6*8点阵*/
//	OLED_ShowString(16, 18, "Hello World!", OLED_6X8);
//
//	/*在(0, 28)位置显示数字12345，长度为5，字体大小为6*8点阵*/
//	OLED_ShowNum(0, 28, 12345, 5, OLED_6X8);
//
//	/*在(40, 28)位置显示有符号数字-66，长度为2，字体大小为6*8点阵*/
//	OLED_ShowSignedNum(40, 28, -66, 2, OLED_6X8);
//
//	/*在(70, 28)位置显示十六进制数字0xA5A5，长度为4，字体大小为6*8点阵*/
//	OLED_ShowHexNum(70, 28, 0xA5A5, 4, OLED_6X8);
//
//	/*在(0, 38)位置显示二进制数字0xA5，长度为8，字体大小为6*8点阵*/
//	OLED_ShowBinNum(0, 38, 0xA5, 8, OLED_6X8);
//
//	/*在(60, 38)位置显示浮点数字123.45，整数部分长度为3，小数部分长度为2，字体大小为6*8点阵*/
//	OLED_ShowFloatNum(60, 38, 123.45, 3, 2, OLED_6X8);
//
//	/*在(0, 48)位置显示汉字串"你好，世界。"，字体大小为固定的16*16点阵*/
//	OLED_ShowChinese(0, 48, "你好，世界。");
//
//	/*在(96, 48)位置显示图像，宽16像素，高16像素，图像数据为Diode数组*/
//	OLED_ShowImage(96, 48, 16, 16, Diode);
//
//	/*在(96, 18)位置打印格式化字符串，字体大小为6*8点阵，格式化字符串为"[%02d]"*/
//	OLED_Printf(96, 18, OLED_6X8, "[%02d]", 6);
//
//	/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//	OLED_Update();
//
//	/*延时3000ms，观察现象*/
//	Delay_ms(3000);
//
//	/*清空OLED显存数组*/
//	OLED_Clear();
//
//	/*在(5, 8)位置画点*/
//	OLED_DrawPoint(5, 8);
//
//	/*获取(5, 8)位置的点*/
//	if (OLED_GetPoint(5, 8))
//	{
//		/*如果指定点点亮，则在(10, 4)位置显示字符串"YES"，字体大小为6*8点阵*/
//		OLED_ShowString(10, 4, "YES", OLED_6X8);
//	}
//	else
//	{
//		/*如果指定点未点亮，则在(10, 4)位置显示字符串"NO "，字体大小为6*8点阵*/
//		OLED_ShowString(10, 4, "NO ", OLED_6X8);
//	}
//
//	/*在(40, 0)和(127, 15)位置之间画直线*/
//	OLED_DrawLine(40, 0, 127, 15);
//
//	/*在(40, 15)和(127, 0)位置之间画直线*/
//	OLED_DrawLine(40, 15, 127, 0);
//
//	/*在(0, 20)位置画矩形，宽12像素，高15像素，未填充*/
//	OLED_DrawRectangle(0, 20, 12, 15, OLED_UNFILLED);
//
//	/*在(0, 40)位置画矩形，宽12像素，高15像素，填充*/
//	OLED_DrawRectangle(0, 40, 12, 15, OLED_FILLED);
//
//	/*在(20, 20)、(40, 25)和(30, 35)位置之间画三角形，未填充*/
//	OLED_DrawTriangle(20, 20, 40, 25, 30, 35, OLED_UNFILLED);
//
//	/*在(20, 40)、(40, 45)和(30, 55)位置之间画三角形，填充*/
//	OLED_DrawTriangle(20, 40, 40, 45, 30, 55, OLED_FILLED);
//
//	/*在(55, 27)位置画圆，半径8像素，未填充*/
//	OLED_DrawCircle(55, 27, 8, OLED_UNFILLED);
//
//	/*在(55, 47)位置画圆，半径8像素，填充*/
//	OLED_DrawCircle(55, 47, 8, OLED_FILLED);
//
//	/*在(82, 27)位置画椭圆，横向半轴12像素，纵向半轴8像素，未填充*/
//	OLED_DrawEllipse(82, 27, 12, 8, OLED_UNFILLED);
//
//	/*在(82, 47)位置画椭圆，横向半轴12像素，纵向半轴8像素，填充*/
//	OLED_DrawEllipse(82, 47, 12, 8, OLED_FILLED);
//
//	/*在(110, 18)位置画圆弧，半径15像素，起始角度25度，终止角度125度，未填充*/
//	OLED_DrawArc(110, 18, 15, 25, 125, OLED_UNFILLED);
//
//	/*在(110, 38)位置画圆弧，半径15像素，起始角度25度，终止角度125度，填充*/
//	OLED_DrawArc(110, 38, 15, 25, 125, OLED_FILLED);
//
//	/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//	OLED_Update();
//
//	/*延时3000ms，观察现象*/
//	Delay_ms(3000);
//
//	while (1)
//	{
//		for (uint8_t i = 0; i < 4; i ++)
//		{
//			/*将OLED显存数组部分数据取反，从(0, i * 16)位置开始，宽128像素，高16像素*/
//			OLED_ReverseArea(0, i * 16, 128, 16);
//
//			/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//			OLED_Update();
//
//			/*延时1000ms，观察现象*/
//			Delay_ms(1000);
//
//			/*把取反的内容翻转回来*/
//			OLED_ReverseArea(0, i * 16, 128, 16);
//		}
//
//		/*将OLED显存数组全部数据取反*/
//		OLED_Reverse();
//
//		/*调用OLED_Update函数，将OLED显存数组的内容更新到OLED硬件进行显示*/
//		OLED_Update();
//
//		/*延时1000ms，观察现象*/
//		Delay_ms(1000);
//	}