@.str = private constant[4 x i8] c"%d\0A\00"
@.str1 = private constant[4 x i8] c"%f\0A\00"
declare i32 @printf(i8*, ...)
@x = global i32 0
define i32 @main(){
%a = alloca i32
%b = alloca i32
%c = alloca double
%d = alloca double
%e = alloca [ x double], align 16
%f = alloca [ÀTPÿ x i32], align 16
%1 = getelementptr inbounds [15 x i32]* %f, i32 0, i64 3
%2 = fptosi double 2.000000e+00 to i32
store i32 %2, i32* %1
%3 = getelementptr inbounds [15 x i32]* %f, i32 0, i64 3
%4= load i32* %3
%5 = sitofp i32 %4 to double
%6 = fmul double %5, 2.000000e+00
%7 = fadd double 1.000000e+00, %6
%8 = fsub double 3.000000e+00, %7
%9 = fptosi double %8 to i32
store i32 %9, i32* %a
%10 = load i32* %a
%11 = sitofp i32 %10 to double
%12 = load i32* %b
%13 = sitofp i32 %12 to double
%14 = fptosi double %13 to i32 
%15 = fptosi double %11 to i32 
%16 = or  i32 %14, %15
%17 = icmp eq i32 1, %16
br i1 %17, label %18 , label %28
%19 = load i32* %b
%20 = sitofp i32 %19 to double
%21 = fmul double %20, 2.000000e+00
%22 = load i32* %a
%23 = sitofp i32 %22 to double
%24 = fmul double %23, 10.000000e+00
%25 = fadd double %24, %21
%26 = fsub double 3.000000e+00, %25
%27 = fptosi double %26 to i32
store i32 %27, i32* %a
br label %34
%29 = load i32* @x
%30 = sitofp i32 %29 to double
%31 = load double* %d
%32 = fadd double 3.3000000e+00, %31
%33 = fadd double %32, %30
store double %33, double* %d
br label %34
br label %35
br label %36
%37 = load i32* %a
%38 = sitofp i32 %37 to double
%39 = fcmp ogt double %38, 3.000000e+00
%40 = zext i1 %39 to i32
%41 = icmp eq i32 1, %40
br i1 %41, label %42 , label %47
%43 = load i32* %a
%44 = sitofp i32 %43 to double
%45 = fadd double 1.000000e+00, %44
%46 = fptosi double %45 to i32
store i32 %46, i32* %a
br label %36
%48 = load i32* %a
%49 = sitofp i32 %48 to double
%50 = fptosi double %49 to i32
ret i32 %50
}
