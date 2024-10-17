# FROM hsireg.azurecr.io/edgeapi:3.6.1
FROM python:3.10-slim-buster

RUN apt-get update && \
	apt-get install -y --allow-unauthenticated openssh-server g++ && \ 
	mkdir /var/run/sshd && \
	echo 'root:toor' | chpasswd && \
	# permit root login
	sed -i -E 's/#\s*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
	# fixed session
	sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd && \ 
	apt-get clean
RUN apt-get install -y wget
COPY requirements.txt .
COPY EdgeAPI/ /EdgeAPI/
RUN apt-get update && \
	apt-get install -y --allow-unauthenticated curl  nano && \
	echo "/EdgeAPI/libs" > /etc/ld.so.conf.d/EdgeAPI.conf && \
	ldconfig
RUN pip3 install -r requirements.txt
RUN rm -rf /root/.cache
#RUN pip install --ignore-installed --upgrade tensorflow-2.8.0-cp310-cp310-linux_x86_64.whl
RUN service ssh start
EXPOSE 22

VOLUME /home/test
WORKDIR /home/test

CMD  ["/usr/sbin/sshd", "-D"]
