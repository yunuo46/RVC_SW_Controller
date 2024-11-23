# 변수 정의
CC = gcc               # 컴파일러
CFLAGS = -pthread      # 컴파일 옵션
SRC = main.c            # 소스 파일
OBJ = main.o            # 오브젝트 파일
EXEC = main             # 최종 실행 파일명

# 기본 타겟
all: $(EXEC)

# 실행 파일 생성 규칙
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

# 오브젝트 파일 생성 규칙
$(OBJ): $(SRC) rvc_sw.h
	$(CC) $(CFLAGS) -c $(SRC)

# 빌드된 파일 삭제 규칙
clean:
	rm -f $(OBJ) $(EXEC)
