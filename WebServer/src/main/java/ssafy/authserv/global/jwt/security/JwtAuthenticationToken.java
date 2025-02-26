package ssafy.authserv.global.jwt.security;

import lombok.Getter;
import org.springframework.security.authentication.AbstractAuthenticationToken;
import org.springframework.security.core.GrantedAuthority;

import java.util.Collection;

@Getter
public class JwtAuthenticationToken extends AbstractAuthenticationToken {
    private final MemberLoginActive principal;
    private final Object credentials;

    public JwtAuthenticationToken(MemberLoginActive principal, Object credentials, Collection<? extends GrantedAuthority> authorities) {
        super(authorities);
        this.principal = principal;
        this.credentials = credentials;
        super.setAuthenticated(true);
    }

    @Override
    public Object getCredentials() { return this.credentials; }

    @Override
    public Object getPrincipal() { return this.principal; }

}
