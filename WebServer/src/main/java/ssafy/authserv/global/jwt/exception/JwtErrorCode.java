package ssafy.authserv.global.jwt.exception;

import lombok.AllArgsConstructor;
import lombok.Getter;
import org.springframework.http.HttpStatus;

@Getter
@AllArgsConstructor
public enum JwtErrorCode {
    EXPIRED_TOKEN(HttpStatus.UNAUTHORIZED, "토큰이 만료되었거나 없습니다. 다시 로그인 후 이용해 주시기 바랍니다." ),
    INVALID_TOKEN(HttpStatus.BAD_REQUEST, "사용할 수 없는 토큰입니다. 다시 로그인 후 이용해주시기 바랍니다."),
    SIGNATURE_INVALID(HttpStatus.FORBIDDEN, "토큰의 서명 검증에 실패했습니다. 다시 로그인 후 이용해 주시기 바랍니다."),
    INVALID_CLAIMS(HttpStatus.UNAUTHORIZED, "다시 로그인 하세요");
    private final HttpStatus httpStatus; // 에러 상황에 해당하는 HTTP 상태
    private final String errorMessage; // 에러 상황을 설명하는 메시지
}
