# Python 실행을 위한 설정
PYTHON = python3
SCRIPT = srcs/client.py

# 기본 규칙 (실행)
all: run

# Python 파일 실행
run:
	$(PYTHON) $(SCRIPT)

# 실행 후 클린업 (필요시 추가)
clean:
	@echo "No cleanup required for Python script."

# 재실행
re: clean all
