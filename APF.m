close('all'), clear, clc;

[x,fs] = audioread("Exemplu7.wav");
x = x*0.5;

t = 0:1/fs:length(x)/fs-1/fs;

%figure(1);
%plot(t,x);
%xlabel("timp [s]"), ylabel("amplitudine"), title("Semnalul original");

fc = 0.1;
fb = 0.022*fs;
K = tan(pi*fc / fs);
Q = 0.5;

a = zeros(1,3);
b = zeros(1,3);
aux = K^2*Q + K + Q;

% Coeficienti pentru FTJ
% b(1) = K^2*Q / aux;
% b(2) = 2 * b(1);
% b(3) = b(1);
% a(2) = 2*Q*(K^2-1) / aux;
% a(3) = (aux - 2*K) / aux;

% Coeficienti pentru FTS
% b(1) = Q / aux;
% b(2) = -2 * b(1);
% b(3) = b(1);
% a(2) = 2*Q*(K^2-1) / aux;
% a(3) = (aux - 2*K) / aux;

%coeficienti pentru 2nd order ALLPASS filter
b(1) = (aux-2*K)/aux;
b(2) = 2*Q*(K^2-1)/aux;
b(3) = 1;
a(2) = b(2);
a(3) = b(1);

%coeficient c si d pentru 2nd order ALLPASS filter
c = (tan(pi*fb/fs) - 1) / (tan(pi*fb/fs) + 1);
d = -cos(2*pi*fc/fs);

% operatia de filtrare

y = zeros(length(x), 1);

% Metoda 1
% xh = zeros(1,3);
% for n=1:length(x)
%     xh(1) = x(n) - a(2)*xh(2) - a(3)*xh(3);  
%     y(n) = xh(1)*b(1) + xh(2)*b(2) + xh(3)*b(3);
%     xh(3) = xh(2);
%     xh(2) = xh(1);
% end

% Metoda 2
% xh = zeros(1,3);
% for n = 1:length(x)
%    xh(3) = x(n) - d*(1-c)*xh(2) + c*xh(1);
%    y(n) = -c*xh(3) + d*(1-c)*xh(2) + xh(1);
%    xh(3) = xh(2);
%    xh(2) = xh(1)
% end

% Metoda 3
xh = zeros(1,length(x));
for n = 3:length(x)
   xh(n) = x(n) - d*(1-c)*xh(n-1) + c*xh(n-2);
   y(n) = -c*xh(n) + d*(1-c)*xh(n-1) + xh(n-2);
end

sound(x(1:length(x)), fs); 
pause(length(x)/fs + 1);
sound(y(1:length(x)), fs);

X = fft(x, 2^10);
Y = fft(y, 2^10);

f = 0 : fs / length(X) : fs/2 - fs/length(X);
figure(2);
semilogx(f, 20*log10(abs(X(1:end/2))));
hold on;
semilogx(f, 20*log10(abs(Y(1:end/2))));
legend("Semnalul original", "Semnalul filtrat");
xlabel("Frecventa [Hz]"), ylabel("Amplitudine [dB]"), title("Spectrul semnalului");


