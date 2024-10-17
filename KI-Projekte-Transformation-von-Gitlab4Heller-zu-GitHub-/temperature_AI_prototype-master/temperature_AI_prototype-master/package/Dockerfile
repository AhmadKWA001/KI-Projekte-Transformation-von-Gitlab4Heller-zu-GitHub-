FROM python:3.8-slim-buster

#AppSDK python version dependencies
COPY EdgeAPI/ /EdgeAPI/
COPY main.py/ /opt/
COPY database_service.py/ /opt/
COPY parameter_handler.py/ /opt/
COPY specifigConfig_reader.py/ /opt/

RUN apt-get update && \
	echo "/EdgeAPI/libs" > /etc/ld.so.conf.d/EdgeAPI.conf && \
	ldconfig


RUN apt-get update && \
	apt-get install -y --allow-unauthenticated openssh-server g++ && \
	mkdir /var/run/sshd && \
	echo 'root:ai_model' | chpasswd && \
	# permit root login
	sed -i -E 's/#\s*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
	# fixed session
	sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd && \
	apt-get clean
COPY requirements.txt .
RUN pip3 install -r requirements.txt

RUN service ssh start
EXPOSE 22

WORKDIR /opt

CMD  python3 main.py
