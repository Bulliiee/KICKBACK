package ssafy.authserv.domain.record.service;

import org.springframework.data.domain.Page;
import ssafy.authserv.domain.member.entity.Member;
import ssafy.authserv.domain.record.dto.SoccerRankingInfo;

public interface RankingService {
    Page<Member> getSoccerRecords(int pageNum);

    Page<SoccerRankingInfo> getSoccerRanking(int pageNum);
}