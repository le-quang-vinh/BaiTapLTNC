Sinh viên: Lê Quang Vinh 		Sinh ngày: 16/07/2006	
Ngành học: Công nghệ thông tin 		Lớp: K69I-IT3	
Tên game : Game bình thường cơ bản 	
Lối chơi, logic của game:
               Nhân vật di chuyển bằng các phím mũi tên tránh các quái vật.
              Quái vật ban đầu chỉ có một con khi chạm vào tường thì tăng lên một con tối đa là 5
              Nhân vật có thể tích lũy năng lượng và sử dụng năng lượng tiêu diệt quái vật
              Khi chạm vào quái vật nhân vật sẽ chết và hiện game over	
Đồ họa, âm thanh:
 Đồ họa không có 
                Âm thanh nhạc nền 
                âm thanh khi bắn chưởng 	
Cấu trúc của project game:	
Sources init.cpp các hàm xử lý âm thanh
              input.cpp các hàm xử lý dữ liệu vào từ chuột và bàn phím 
              logicgame.cpp các hàm xử lý logic game
              main.cpp file chính chạy game
              render.cpp render hình ảnh 
Headers config.h  khai báo các hằng số cố định 
              global.h khai báo các biến toàn cục 
              init.h khai báo hàm xử lý âm thanh 
             input.h khai báo hàm xử lý dữ liệu đầu vào 
             logicgame.h khai báo các hàm xử lý logic game
             render.h khai báo các hàm render                   	
Các chức năng đã cài được cho game:	
                  Tích lũy năng lượng 	
                  Sử dụng năng lượng tấn công quái vật 	
                  Quái vật nhân đôi khi chạm vào màn hình 	
                  Tính năng di chuyển né tránh quái vật
